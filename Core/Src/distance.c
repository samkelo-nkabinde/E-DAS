/*
 * distance.c
 *
 *  Created on: Mar 11, 2026
 *      Author: Samkelo Nkabinde
 */

#include "distance.h"

kalman_filter_t kf_distance;
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

float compute_distance(uint32_t pulse_width)
{
	return (float)(pulse_width / 58);
}

bool proximity_warning(void)
{
	return average_distance <= 10.0f;
}

