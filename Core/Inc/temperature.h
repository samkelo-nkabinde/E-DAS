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

typedef struct
{
	float raw;
	uint32_t last_time;
	uint32_t pulse_count;
	float filtered;
}temperature_t;

extern temperature_t temperature;

void temperature_init();
bool get_pulse_count(void);
void compute_temperature(void);
bool temperature_is_high(void);

#endif /* INC_TEMPERATURE_H_ */
