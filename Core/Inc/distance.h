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
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>

#include "kalman_filter.h"

extern float average_distance;

uint32_t get_pulse_width(void);
float compute_distance(uint32_t pulse_width);
bool proximity_warning(void);

#endif /* INC_DISTANCE_H_ */
