/*
 * uart_system.c
 *
 *  Created on: Apr 18, 2026
 *      Author: Samkelo Nkabinde
 */

#include "uart_system.h"

void uart_system_update(void)
{
	char command[64];

	if(UART_get_command(&g_uart2, command, sizeof(command)) == false)
		return;

	command_e type = command_type(command);

	switch(type)
	{
		case STAT_COMMAND:
			stats_transmit_all();
			break;

		case SETWARN_COMMAND:
			int c, y;
			if (sscanf(command, "@SetWarn %d=%d&", &c, &y) == 2)
				update_warning(c, y);
			break;

		case LOG_COMMAND:
			break;

		case DUMP_COMMAND:
			break;

		case SFD_COMMAND:
			float liters, distance;
			if(sscanf(command, "@SFD %f;%f&", &liters, &distance) == 2)
			{
				last_liters = liters;
				last_distance = distance;
				compute_fuel_efficiency();
			}
			break;

		case RFE_COMMAND:
			char fuel_data[64];
			sprintf(fuel_data, "Fuel Eff: %.1f km/L %.1f L/100km\n&\n", km_per_l, l_per_100km);
			stats_transmit_one(STAT_DATE);
			UART_transmit(&g_uart2, (uint8_t *)fuel_data, strlen(fuel_data));
			break;

		case CLF_COMMAND:
			break;

		case SETRST_COMMAND:
			uint32_t year, month, day;
			uint32_t hour, minute, second;

			if(sscanf(command, "@SetRTC %lu/%lu/%lu;%lu:%lu:%lu&",
			       &year, &month, &day,
			       &hour, &minute, &second) == 6)
			{
				date_set(year, month, day, hour, minute, second);
			}
			break;

		default:
			break;
	}

}

void update_warning(uint32_t c, uint32_t y)
{
	switch(c)
	{
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:
			dist_warning = y;
			break;
		case 5:
			temp_warning = y;
			break;
		default:
			break;
	}
	return;
}
