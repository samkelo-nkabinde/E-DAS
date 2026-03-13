/*
 * distance.c
 *
 *  Created on: Mar 11, 2026
 *      Author: Samkelo Nkabinde
 */

#include "distance.h"

uint32_t distance_data[DISTANCE_DATA_WINDOW] = {0};
uint8_t distance_data_index = 0;
uint32_t average_distance = UINT_MAX;


uint32_t get_pulse_width()
{
	uint32_t pulse_width_start = 0;
	uint32_t pulse_width_end = 0;

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);

	__HAL_TIM_SET_COUNTER(&htim1, 0);
	while (__HAL_TIM_GET_COUNTER(&htim1) < 10);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

	uint32_t timeout = 0xFFFF;
	while (!(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)) && timeout--);
	pulse_width_start = __HAL_TIM_GET_COUNTER(&htim1);

	timeout = 0xFFFF;
	while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) && timeout--);
	pulse_width_end = __HAL_TIM_GET_COUNTER(&htim1);

	return (pulse_width_end - pulse_width_start);

}

void compute_average_distance(uint32_t pulse_width)
{
	uint32_t current_distance = pulse_width / 58;

	if(current_distance > 400)
	    return;

	distance_data[distance_data_index] = current_distance;
	distance_data_index = (distance_data_index + 1) % DISTANCE_DATA_WINDOW;

	uint32_t sum = 0;
	uint8_t count = 0;

	for(int i = 0; i < DISTANCE_DATA_WINDOW; i++)
	{
		if(distance_data[i] != 0)
		{
			sum += distance_data[i];
			count++;
		}
	}

	if(count > 0)
		average_distance = sum / count;

	return;
}

bool proximity_warning(void)
{
	return average_distance <= 10;
}

