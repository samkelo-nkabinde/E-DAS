/*
 * temperature.h
 *
 *  Created on: Mar 9, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_

#include "stm32f4xx_hal.h"
#include "main.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>



extern volatile uint32_t pulse_count;
extern volatile uint32_t last_pulse_tick;

#define TEMPERATURE_DATA_WINDOW 5

extern int32_t temperature_data[TEMPERATURE_DATA_WINDOW];
extern uint8_t temperature_data_index;
extern int32_t average_temperature;

uint32_t get_final_pulse_count(uint32_t current_time);
void compute_average_temperature(uint32_t final_pulse_count);
void transimit_temperature(void);

#endif /* INC_TEMPERATURE_H_ */
