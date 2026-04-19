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

#include "kalman_filter.h"

extern bool temp_warning;
extern volatile uint32_t pulse_count;

typedef struct
{
	uint32_t last_time;
	uint32_t pulse_count;
	float raw;
	float filtered;
	bool warning;
}temperature_t;

extern temperature_t temperature;

void temperature_init(void);
bool get_pulse_count(void);
void compute_temperature(void);
void update_temperature(void);

#endif /* INC_TEMPERATURE_H_ */
