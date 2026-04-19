/*
 * uart_system.h
 *
 *  Created on: Apr 18, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_UART_SYSTEM_H_
#define INC_UART_SYSTEM_H_

#include <stdio.h>

#include "uart_handle.h"
#include "command.h"
#include "date.h"
#include "stats.h"
#include "distance.h"
#include "temperature.h"
#include "fuel.h"

void uart_system_update(void);
void update_warning(uint32_t c, uint32_t y);
#endif /* INC_UART_SYSTEM_H_ */
