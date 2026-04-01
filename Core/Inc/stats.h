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
#include "temperature.h"
#include "distance.h"

#define SIZE 22
#define NUMBER_OF_STATS 14

extern bool alarm_checking;
extern volatile uint8_t stats_requested;
typedef enum
{
	DATE = 0,
	DISTANCE,
	TEMPERATURE,
	LIGHT,
	X_ACCELARATION,
	Y_ACCELARATION,
	Z_ACCELARATION,
	UNSAFE_DRIVING,
	IMPACT_DETECTED,
	LOW_LIGHT_WARNING,
	PROXIMITY_WARNING,
	HIGH_TEMPERATURE,
	GPS_LATITUDE,
	GPS_LONGITUDE

}Stat_type_e;

extern char stat_data[NUMBER_OF_STATS][SIZE];

void stats_init(void);
void update_stat(Stat_type_e stat);
void transimit_stat(Stat_type_e stat);
void transimit_all_stats(void);
void update_date_stat(void);
#endif /* INC_STATS_H_ */
