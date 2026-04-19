/*
 * distance.h
 *
 *  Created on: Mar 11, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_DISTANCE_H_
#define INC_DISTANCE_H_

#include "main.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "kalman_filter.h"

typedef struct
{
	float raw;
	float filtered;
	bool warning;
}distance_t;

extern distance_t distance;
extern bool dist_warning;

void distance_init(void);
uint32_t get_pulse_width(void);
float compute_distance(uint32_t pulse_width);
void update_distance(void);

#endif /* INC_DISTANCE_H_ */
