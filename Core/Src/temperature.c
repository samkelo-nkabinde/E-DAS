/*
 * temperature.c
 *
 *  Created on: Mar 9, 2026
 *      Author: Samkelo Nkabinde
 */

#include "temperature.h"

volatile uint32_t pulse_count = 0;

float average_temperature = 25.0f;

uint32_t get_final_pulse_count(void)
{
    uint32_t final_pulse_count;

    __disable_irq();
    final_pulse_count = pulse_count;
    pulse_count = 0;
    __enable_irq();

    return final_pulse_count;
}

float compute_temperature(uint32_t final_pulse_count)
{

    float current_temperature = (((int32_t)final_pulse_count * 2560) / 4096) - 500;
    return current_temperature / 100.0f;
}

bool temperature_is_high(void)
{
    return average_temperature > 30;
}
