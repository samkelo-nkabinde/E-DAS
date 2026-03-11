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

#define DISTANCE_DATA_WINDOW 5

extern uint32_t distance_data[DISTANCE_DATA_WINDOW];
extern uint8_t distance_data_index;
extern uint32_t average_distance;

uint32_t get_pulse_width(void);
void compute_average_distance(void);

#endif /* INC_DISTANCE_H_ */
