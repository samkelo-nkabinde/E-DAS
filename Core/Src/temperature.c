/*
 * temperature.c
 *
 *  Created on: Mar 9, 2026
 *      Author: Samkelo Nkabinde
 */

#include "temperature.h"

 bool temp_warning = 0;
volatile uint32_t pulse_count = 0;
temperature_t temperature;
static kalman_filter_t kf_temperature;

void temperature_init()
{
	temperature.last_time = HAL_GetTick();
	temperature.pulse_count = pulse_count;
	temperature.raw = 25.0f;
	temperature.filtered = temperature.raw;
	temperature.warning = temp_warning ? temp_warning : temperature.filtered >= 30.0f;
	kalman_init(&kf_temperature, temperature.raw);

	return;
}

bool get_pulse_count(void)
{
    __disable_irq();
    pulse_count = 0;
    __enable_irq();

    uint32_t start_time = HAL_GetTick();


    while(pulse_count == 0)
    {
        if(HAL_GetTick() - start_time > 160) return false;
    }


    uint32_t last_count = 0;
    uint32_t timer = HAL_GetTick();

    while(HAL_GetTick() - timer < 10)
    {
        if(pulse_count != last_count)
        {
        	last_count = pulse_count;
            timer = HAL_GetTick();
        }

    }

    temperature.pulse_count = pulse_count;
    return true;
}

void compute_temperature(void)
{
	if(temperature.pulse_count > 1000)
		temperature.raw = ((temperature.pulse_count / 4096.0f) * 256.0f - 50.0f);
	return;
}

void update_temperature(void)
{
	get_pulse_count();
	compute_temperature();
	temperature.filtered = kalman_update(&kf_temperature, temperature.raw);
	temperature.warning = temp_warning ? temp_warning : temperature.filtered >= 30.0f;
}

