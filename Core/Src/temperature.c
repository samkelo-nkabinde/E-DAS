/*
 * temperature.c
 *
 *  Created on: Mar 9, 2026
 *      Author: Samkelo Nkabinde
 */

#include "temperature.h"

volatile uint32_t pulse_count = 0;
volatile uint32_t last_pulse_tick = 0;

int32_t temperature_data[TEMPERATURE_DATA_WINDOW] = {0};
uint8_t temperature_data_index = 1;
int32_t average_temperature = INT_MIN;

uint32_t get_final_pulse_count(uint32_t current_time)
{
	uint32_t final_pulse_count= 0;

	if (!(current_time - last_pulse_tick > 80))
		return final_pulse_count;

	 __disable_irq();
	 final_pulse_count = pulse_count;
	 pulse_count = 0;
	 __enable_irq();

	 return final_pulse_count;
}

void compute_average_temperature(uint32_t final_pulse_count)
{
	if (final_pulse_count < 1000)
		return;

	  int32_t current_temperature = ((int32_t)final_pulse_count * 2560 / 4096) - 500;

	  temperature_data[temperature_data_index] = current_temperature;
	  temperature_data_index = (temperature_data_index + 1) % TEMPERATURE_DATA_WINDOW;

	  int32_t sum = 0;
	  int count = 0;

	  for(int i = 0; i < TEMPERATURE_DATA_WINDOW; i++)
	  {
		  if(temperature_data[i] != 0)
		  {
			  sum += temperature_data[i];
			  count++;
		  }
	  }

	  average_temperature = sum / count;
}

void transimit_temperature(void)
{
	char message[64];
	sprintf(message, "@%d.%d&\n", (int32_t)(average_temperature / 10), (int32_t)abs(average_temperature % 10));
	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), 100);
}

