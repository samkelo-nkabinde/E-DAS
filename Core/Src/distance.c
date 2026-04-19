/*
 * distance.c
 *
 *  Created on: Mar 11, 2026
 *      Author: Samkelo Nkabinde
 */

#include "distance.h"

distance_t distance;
bool distance_external_warning = 0;
static kalman_filter_t kf_distance;

static uint32_t get_pulse_width();
static float compute_distance(uint32_t pulse_width);

void distance_init(void)
{
	distance.raw = 100.04f;
	distance.filtered = distance.raw;
	distance.warning = distance_external_warning ? distance_external_warning : distance.filtered < 10.0;
	kalman_init(&kf_distance, distance.raw);
	HAL_TIM_Base_Start(&htim1);
	return;
}

static uint32_t get_pulse_width()
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

static float compute_distance(uint32_t pulse_width)
{
	return (float)(pulse_width / 58);
}

void update_distance(void)
{
	distance.raw = compute_distance(get_pulse_width());
	distance.filtered = kalman_update(&kf_distance, distance.raw);
	distance.warning = distance_external_warning ? distance_external_warning : distance.filtered < 10.0;
}


