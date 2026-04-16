/*
 * stats.c
 *
 *  Created on: Mar 11, 2026
 *      Author: Samkelo Nkabinde
 */

#include "stats.h"

char stat_data[NUMBER_OF_STATS][SIZE];

volatile uint8_t stats_requested = 0;
bool alarm_checking = false;

void update_date_stat(void)
{
    RTC_DateTypeDef sDate;
    RTC_TimeTypeDef sTime;

    if(HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
        Error_Handler();

    if(HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
        Error_Handler();

    snprintf(stat_data[DATE], SIZE + 1, "@%04d/%02d/%02d %02d:%02d:%02d\n",
             2000 + sDate.Year, sDate.Month, sDate.Date,
             sTime.Hours, sTime.Minutes, sTime.Seconds);
}


void stats_init(void)
{

    snprintf(stat_data[DATE], 22+1, "%-21s\n", "@YYYY/MM/DD HH:MM:SS");
    snprintf(stat_data[DISTANCE], SIZE, "%-9s%6ld.%ld cm\n", "Distance:", (int32_t)(0), (int32_t)(0));
    snprintf(stat_data[TEMPERATURE], SIZE, "%-12s%4ld.%ld C\n", "Temperature:", (int32_t)(0), (int32_t)(0));
    snprintf(stat_data[LIGHT], SIZE, "%-6s%9s lux\n", "Light:", "0000");
    snprintf(stat_data[X_ACCELARATION], SIZE, "%-8s%9s g\n", "X accel:", "0.00");
    snprintf(stat_data[Y_ACCELARATION], SIZE, "%-8s%9s g\n", "Y accel:", "0.00");
    snprintf(stat_data[Z_ACCELARATION], SIZE, "%-8s%9s g\n", "Z accel:", "0.00");
    snprintf(stat_data[UNSAFE_DRIVING], SIZE, "%-15s%5ld\n", "Unsafe driving:", (int32_t)(0));
    snprintf(stat_data[IMPACT_DETECTED], SIZE, "%-17s%3ld\n", "Impact detected:", (int32_t)(0));
    snprintf(stat_data[LOW_LIGHT_WARNING], SIZE, "%-18s%2ld\n", "Low-Light warning:", (int32_t)(0));
    snprintf(stat_data[PROXIMITY_WARNING], SIZE, "%-18s%2ld\n", "Proximity warning:", (int32_t)(0));
    snprintf(stat_data[HIGH_TEMPERATURE], SIZE, "%-17s%3ld\n", "High Temperature:", (int32_t)(0));
    snprintf(stat_data[GPS_LATITUDE], SIZE, "%-8s%12s\n", "GPS lat:", "000.00000");
    snprintf(stat_data[GPS_LONGITUDE], SIZE, "%-9s%11s\n", "GPS long:", "000.00000");

    return;
}

void update_stat(Stat_type_e stat)
{
	switch(stat)
	{
		case DATE:
			break;

		case DISTANCE:
			float distance_cm = average_distance / 1.0f;
			snprintf(stat_data[DISTANCE], SIZE, "%-9s%.1f cm\n", "Distance:", distance_cm);
			break;

		case TEMPERATURE:
			snprintf(stat_data[TEMPERATURE], SIZE, "%-12s%.1f C\n", "Temperature:", 100.0);
			break;

		case HIGH_TEMPERATURE:
			snprintf(stat_data[HIGH_TEMPERATURE], SIZE, "%-17s%3ld\n", "High Temperature:",
					(int32_t)(temperature_is_high()));
			break;

		case PROXIMITY_WARNING:
			snprintf(stat_data[PROXIMITY_WARNING], SIZE, "%-18s%2ld\n", "Proximity warning:",
					(int32_t)(proximity_warning()));
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

void transimit_all_stats(void)
{
	for(uint8_t i = 0; i < NUMBER_OF_STATS; i++)
		transimit_stat(i);

	return;
}
