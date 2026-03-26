/*
 * temperature.h
 *
 *  Created on: Mar 9, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_

#include "main.h"


#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


extern volatile uint32_t pulse_count;
extern volatile uint32_t last_pulse_tick;
extern float average_temperature;

uint32_t get_final_pulse_count(uint32_t current_time);
float compute_temperature(uint32_t final_pulse_count);
bool temperature_is_high(void);

#endif /* INC_TEMPERATURE_H_ */
