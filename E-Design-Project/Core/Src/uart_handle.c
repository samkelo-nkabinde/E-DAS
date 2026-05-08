/*
 * uart_handle.c
 *
 *  Created on: Apr 1, 2026
 *      Author: Samkelo Nkabinde
 */

#include "uart_handle.h"

UART_handle_t g_uart2;

static inline uint16_t dma_write_idx(const UART_handle_t *handle)
{
    return (uint16_t)(UART_RX_BUFFER_SIZE
                      - __HAL_DMA_GET_COUNTER(handle->huart->hdmarx));
}

void UART_init(UART_handle_t *handle, UART_HandleTypeDef *huart)
{
    memset(handle, 0, sizeof(UART_handle_t));
    handle->huart = huart;
    HAL_UART_Receive_DMA(handle->huart, handle->rx_buffer, UART_RX_BUFFER_SIZE);
    __HAL_UART_ENABLE_IT(handle->huart, UART_IT_IDLE);
}

bool UART_transmit(UART_handle_t *handle, const uint8_t *data, uint16_t len)
{
    uint16_t head = handle->tx_head;
    uint16_t tail = handle->tx_tail;
    uint16_t used = (head >= tail) ? (head - tail)
                                   : (UART_TX_BUFFER_SIZE - tail + head);

    if ((UART_TX_BUFFER_SIZE - used - 1) < len) return false;

    for (uint16_t i = 0; i < len; i++)
    {
        handle->tx_buffer[handle->tx_head] = data[i];
        handle->tx_head = (handle->tx_head + 1) % UART_TX_BUFFER_SIZE;
    }

    if (!handle->tx_busy)
    {
        handle->tx_busy = true;
        uint16_t send_len = (handle->tx_head > handle->tx_tail)
                            ? (handle->tx_head - handle->tx_tail)
                            : (UART_TX_BUFFER_SIZE - handle->tx_tail);

        HAL_UART_Transmit_DMA(handle->huart,
                              &handle->tx_buffer[handle->tx_tail],
                              send_len);
    }

    return true;
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance != g_uart2.huart->Instance) return;

    UART_handle_t *handle = &g_uart2;
    handle->tx_tail = (handle->tx_tail + huart->TxXferSize) % UART_TX_BUFFER_SIZE;

    if (handle->tx_tail != handle->tx_head)
    {
        uint16_t send_len = (handle->tx_head > handle->tx_tail)
                            ? (handle->tx_head - handle->tx_tail)
                            : (UART_TX_BUFFER_SIZE - handle->tx_tail);

        HAL_UART_Transmit_DMA(handle->huart,
                              &handle->tx_buffer[handle->tx_tail],
                              send_len);
    }
    else
        handle->tx_busy = false;
}


uint16_t UART_receive(UART_handle_t *handle, uint8_t *out_buf, uint16_t max_len)
{
    uint16_t available = UART_rx_available(handle);
    uint16_t to_copy   = (available < max_len) ? available : max_len;

    for (uint16_t i = 0; i < to_copy; i++)
    {
        out_buf[i]    = handle->rx_buffer[handle->rx_ptr];
        handle->rx_ptr = (handle->rx_ptr + 1) % UART_RX_BUFFER_SIZE;
    }

    return to_copy;
}

bool UART_get_command(UART_handle_t *handle, char *out_cmd, uint16_t max_len)
{
    uint16_t w    = dma_write_idx(handle);
    uint16_t scan = handle->rx_ptr;
    uint16_t len  = 0;
    bool found    = false;

    while (scan != w)
    {
        uint8_t c = handle->rx_buffer[scan];
        if (c == '\r' || c == '\n') { found = true; break; }
        scan = (scan + 1) % UART_RX_BUFFER_SIZE;
        if (++len >= max_len - 1) break;
    }

    if (!found) return false;

    len = 0;
    while (handle->rx_ptr != w && len < max_len - 1)
    {
        uint8_t c  = handle->rx_buffer[handle->rx_ptr];
        handle->rx_ptr = (handle->rx_ptr + 1) % UART_RX_BUFFER_SIZE;

        if (c == '\r' || c == '\n')
        {
            if (len == 0) continue;
            out_cmd[len] = '\0';
            return true;
        }
        out_cmd[len++] = (char)c;
    }

    return false;
}

uint16_t UART_rx_available(const UART_handle_t *handle)
{
    uint16_t w = dma_write_idx(handle);
    uint16_t r = handle->rx_ptr;
    return (w >= r) ? (w - r) : (UART_RX_BUFFER_SIZE - r + w);
}

void UART_Irq_Handler(UART_handle_t *handle)
{
    if (__HAL_UART_GET_FLAG(handle->huart, UART_FLAG_IDLE))
        __HAL_UART_CLEAR_IDLEFLAG(handle->huart);
}

