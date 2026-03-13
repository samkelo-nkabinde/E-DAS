/*
 * temperature.c
 *
 *  Created on: Mar 9, 2026
 *      Author: Samkelo Nkabinde
 */

#include "temperature.h"

volatile uint32_t pulse_count = 0;

int32_t temperature_data[TEMPERATURE_DATA_WINDOW] = {0};
uint8_t temperature_data_index = 0;
int32_t average_temperature = 250;

uint32_t get_final_pulse_count(uint32_t current_time)
{
    static uint32_t last_sample_time = 0;


    if ((current_time - last_sample_time) < 1000)
    {
        return 0;
    }

    last_sample_time = current_time;

    uint32_t final_pulse_count;

    __disable_irq();
    final_pulse_count = pulse_count;
    pulse_count = 0;
    __enable_irq();

    return final_pulse_count;
}

void compute_average_temperature(uint32_t final_pulse_count)
{

    if (final_pulse_count == 0)
        return;


    int32_t current_temperature = (((int32_t)final_pulse_count * 2560) / 4096) - 500;

    current_temperature = current_temperature / 3;

    temperature_data[temperature_data_index] = current_temperature;
    temperature_data_index = (temperature_data_index + 1) % TEMPERATURE_DATA_WINDOW;

    int32_t sum = 0;
    for(int i = 0; i < TEMPERATURE_DATA_WINDOW; i++)
    {
        sum += temperature_data[i];
    }

    average_temperature = sum / TEMPERATURE_DATA_WINDOW;
}

bool temperature_is_high(void)
{
    return average_temperature / 100 > 30;
}
