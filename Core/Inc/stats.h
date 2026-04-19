/*
 * stats.h
 *
 *  Created on: Mar 11, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_STATS_H_
#define INC_STATS_H_

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "uart_handle.h"
#include "temperature.h"
#include "distance.h"
#include "date.h"

typedef enum
{
    STAT_DATE = 0,
    STAT_DISTANCE,
    STAT_TEMPERATURE,
    STAT_LIGHT,
    STAT_X_ACCEL,
    STAT_Y_ACCEL,
    STAT_Z_ACCEL,
    STAT_UNSAFE_DRIVING,
    STAT_IMPACT_DETECTED,
    STAT_LOW_LIGHT_WARNING,
    STAT_PROXIMITY_WARNING,
    STAT_HIGH_TEMPERATURE,
    STAT_GPS_LAT,
    STAT_GPS_LONG,
    STAT_COUNT
} Stat_type_e;

void stats_transmit_one(Stat_type_e stat);
void stats_transmit_all();

#endif /* INC_STATS_H_ */
