/*
 * uart_handle.c
 *
 *  Created on: Apr 1, 2026
 *      Author: Samkelo Nkabinde
 */

#include "uart_handle.h"

UART_handle_t g_uart2;

void UART_System_Init(UART_handle_t *handle, UART_HandleTypeDef *huart)
{
    memset(handle, 0, sizeof(UART_handle_t));
    handle->huart = huart;

    HAL_UART_Receive_DMA(handle->huart, handle->rx_buffer, UART_RX_BUFFER_SIZE);

    // Enable IDLE line interrupt to detect end of data packets
    __HAL_UART_ENABLE_IT(handle->huart, UART_IT_IDLE);

    return;
}

bool UART_Transmit_Async(UART_handle_t *handle, const uint8_t *data, uint16_t len)
{
	uint16_t head = handle->tx_head;
	uint16_t tail = handle->tx_tail;

	// Calculate available space
	uint16_t used = (head >= tail) ? (head - tail) : (UART_TX_BUFFER_SIZE - (tail - head));
	if ((UART_TX_BUFFER_SIZE - used - 1) < len) return false; // Buffer Full

	// Copy data into circular buffer
	for (uint16_t i = 0; i < len; i++)
	{
		handle->tx_buffer[handle->tx_head] = data[i];
		handle->tx_head = (handle->tx_head + 1) % UART_TX_BUFFER_SIZE;
	}

	// Trigger DMA if not already transmitting
	if (!handle->tx_busy)
	{
		handle->tx_busy = true;

		// Find contiguous block size to send
		uint16_t send_len;

		if (handle->tx_head > handle->tx_tail)
			send_len = handle->tx_head - handle->tx_tail;
		else
			send_len = UART_TX_BUFFER_SIZE - handle->tx_tail;

		HAL_UART_Transmit_DMA(handle->huart, &handle->tx_buffer[handle->tx_tail], send_len);
	}

	return true;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != g_uart2.huart->Instance) return;

    UART_handle_t *handle = &g_uart2;

    // Update tail based on what was just sent (this handles the wrap-around)
    handle->tx_tail = (handle->tx_tail + huart->TxXferSize) % UART_TX_BUFFER_SIZE;

    if (handle->tx_tail != handle->tx_head)
    {
        // More data is waiting, send next contiguous block
        uint16_t send_len;

        if (handle->tx_head > handle->tx_tail)
            send_len = handle->tx_head - handle->tx_tail;
        else
            send_len = UART_TX_BUFFER_SIZE - handle->tx_tail;

        HAL_UART_Transmit_DMA(handle->huart, &handle->tx_buffer[handle->tx_tail], send_len);
    }
    else
    	handle->tx_busy = false;

    return;
}


bool UART_Get_Command(UART_handle_t *handle, char *out_cmd, uint16_t max_len)
{
    // Determine where DMA is currently writing in the buffer
    // CNDTR counts down from Buffer Size to 0
    uint16_t dma_write_idx = UART_RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(handle->huart->hdmarx);

    uint16_t temp_rx_ptr = handle->rx_ptr;
    uint16_t len = 0;
    bool found_terminator = false;

    // Scan for terminator without moving the global rx_ptr yet
    while (temp_rx_ptr != dma_write_idx)
    {
        uint8_t c = handle->rx_buffer[temp_rx_ptr];
        if (c == '\r' || c == '\n')
        {
            found_terminator = true;
            break;
        }
        temp_rx_ptr = (temp_rx_ptr + 1) % UART_RX_BUFFER_SIZE;
        if (++len >= max_len - 1) break;
    }

    if (!found_terminator) return false;

    // If found, copy data and move the global pointer
    len = 0;
    while (handle->rx_ptr != dma_write_idx && len < max_len - 1)
    {
        uint8_t c = handle->rx_buffer[handle->rx_ptr];
        handle->rx_ptr = (handle->rx_ptr + 1) % UART_RX_BUFFER_SIZE;

        if (c == '\r' || c == '\n')
        {
            if (len == 0) continue; // Skip empty leading terminators
            out_cmd[len] = '\0';
            return true;
        }
        out_cmd[len++] = (char)c;
    }

    return false;
}


void UART_Irq_Handler(UART_handle_t *handle)
{
    if (__HAL_UART_GET_FLAG(handle->huart, UART_FLAG_IDLE))
        __HAL_UART_CLEAR_IDLEFLAG(handle->huart);

    return;
}

