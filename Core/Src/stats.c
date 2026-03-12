/*
 * stats.c
 *
 *  Created on: Mar 11, 2026
 *      Author: Samkelo Nkabinde
 */

#include "stats.h"

char stat_data[NUMBER_OF_STATS][SIZE];

void stats_init(void)
{
	snprintf(stat_data[DATE], SIZE, "%s", "@YYYY/MM/DD HH:MM:SS\n");
	snprintf(stat_data[DISTANCE], SIZE, "%-9s%6ld.%ld cm%c", "Distance:", (int32_t)(0), (int32_t)(0), '\n');
	snprintf(stat_data[TEMPERATURE], SIZE, "%-12s%4ld.%ld C\n", "Temperature:", (int32_t)(0), (int32_t)(0));
	snprintf(stat_data[LIGHT], SIZE, "%-6s%10s lux\n", "Light:", "0000");
	snprintf(stat_data[X_ACCELARATION], SIZE, "%-8s%6s g\n", "X accel:", "0.00");
	snprintf(stat_data[Y_ACCELARATION], SIZE, "%-8s%6s g\n", "Z accel:", "0.00");
	snprintf(stat_data[Z_ACCELARATION], SIZE, "%-8s%6s g\n", "Z accel:", "0.00");
	snprintf(stat_data[UNSAFE_DRIVING], SIZE, "%-15s%5ld\n", "Unsafe driving:", (int32_t)(0));
	snprintf(stat_data[IMPACT_DETECTED], SIZE, "%-17s%3ld\n", "Impact detected:", (int32_t)(0));
	snprintf(stat_data[LOW_LIGHT_WARNING], SIZE, "%-18s%2ld\n", "Low-Light warning:", (int32_t)(0));
	snprintf(stat_data[PROXIMITY_WARNING], SIZE, "%-18s%2ld\n", "Proximity warning:", (int32_t)(0));
	snprintf(stat_data[HIGH_TEMPERATURE], SIZE, "%-17s%3ld\n", "High Temperature:", (int32_t)(0));
	snprintf(stat_data[GPS_LATITUDE], SIZE, "%-8s%12s\n", "GPS lat:", "000.00000");
	snprintf(stat_data[GPS_LONGITUDE], SIZE, "%-9s%9s\n&\n", "GPS long:", "000.00000");

	return;
}

void update_stat(Stat_type_e stat)
{
	switch(stat)
	{
		case DISTANCE:
			snprintf(stat_data[DISTANCE], SIZE, "%-9s%6ld.%ld cm%c", "Distance:",
					(int32_t)(average_distance / 10), (int32_t)abs(average_distance % 10), '\n');
			break;
		case TEMPERATURE:
			snprintf(stat_data[TEMPERATURE], SIZE, "%-12s%4ld.%ld C\n", "Temperature:",
					(int32_t)(average_temperature / 10), (int32_t)abs(average_temperature % 10));
			break;

		case HIGH_TEMPERATURE:
			snprintf(stat_data[HIGH_TEMPERATURE], SIZE, "%-17s%3ld\n", "High Temperature:",
					average_temperature >= 30 ? (int32_t)(1) : (int32_t)(0));
			break;

		case PROXIMITY_WARNING:
			snprintf(stat_data[PROXIMITY_WARNING], SIZE, "%-18s%2ld\n", "Proximity warning:",
					average_distance < 10 ? (int32_t)(1) : (int32_t)(0));
			break;

		default:
			break;
	}
	return;
}

void transimit_stat(Stat_type_e stat)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)stat_data[stat], strlen(stat_data[stat]), 100);
	return;
}

