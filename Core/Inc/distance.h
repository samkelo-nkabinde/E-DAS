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
extern bool distance_external_warning;

void distance_init(void);
void update_distance(void);

#endif /* INC_DISTANCE_H_ */
