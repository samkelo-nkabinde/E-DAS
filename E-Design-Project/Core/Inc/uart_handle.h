/*
 * uart_handle.h
 *
 *  Created on: Mar 30, 2026
 *      Author: Samkelo Nkabinde
 */

// uart_handle.h

#ifndef INC_UART_HANDLE_H_
#define INC_UART_HANDLE_H_

#include "main.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define UART_TX_BUFFER_SIZE 1024
#define UART_RX_BUFFER_SIZE 128

typedef struct
{
    UART_HandleTypeDef *huart;

    uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
    uint8_t rx_buffer[UART_RX_BUFFER_SIZE];

    uint16_t rx_ptr;
    volatile uint16_t tx_head;
    volatile uint16_t tx_tail;
    volatile bool tx_busy;

} UART_handle_t;

extern UART_handle_t g_uart2;

void UART_init(UART_handle_t *handle, UART_HandleTypeDef *huart);
bool UART_transmit(UART_handle_t *handle, const uint8_t *data, uint16_t len);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void UART_Irq_Handler(UART_handle_t *handle);
bool UART_get_command(UART_handle_t *handle, char *out_cmd, uint16_t max_len);

uint16_t UART_receive(UART_handle_t *handle, uint8_t *out_buf, uint16_t max_len);

uint16_t UART_rx_available(const UART_handle_t *handle);

#endif /* INC_UART_HANDLE_H_ */
