/*
 * light.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Samkelo Nkabinde
 */


#include "light.h"

light_t light;
bool light_external_warning = false;

static kalman_filter_t kf_light;

void light_init()
{
	light.raw = 0;
	light.filtered = 0.0f;
	light.lux = 0.0f;
	light.voltage = 0.0f;
	light.warning = light_external_warning ? light_external_warning : 0;
	kalman_init(&kf_light, 1.5f);
	HAL_ADC_Start(&hadc1);
}


void light_update()
{
    HAL_ADC_Start(&hadc1);

    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        light.raw = (uint16_t)HAL_ADC_GetValue(&hadc1);

        light.lux = (0.1497f * light.raw) + 300;// 245.9f;

        if (light.lux < 0.0f)
        {
            light.lux = 0.0f;
        }
    }

    HAL_ADC_Stop(&hadc1);

    light.warning = (light.lux < 300.0f);
}
