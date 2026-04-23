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

void light_update(void)
{
    /* Start ADC */
    if (HAL_ADC_Start(&hadc1) != HAL_OK)
        return;

    /* Wait for conversion */
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        /* Read raw ADC */
        light.raw = (uint16_t)HAL_ADC_GetValue(&hadc1);

        /* Convert to voltage */
        light.voltage = (VREF * (float)light.raw) / (float)ADC_MAX;

        /* Filter */
        light.filtered = kalman_update(&kf_light, light.voltage);

        /* Convert voltage -> lux (calibrated) */
        light.lux = (250.0f * light.filtered) + 365.0f;

        /* Optional clamp (prevents nonsense values) */
        if (light.lux < 0.0f)
            light.lux = 0.0f;
    }
    else
    {
        /* ADC failed → keep last valid values */
        return;
    }

    /* Stop ADC */
    HAL_ADC_Stop(&hadc1);

    /* Warning logic based on requirements */
    light.warning = light_external_warning ||
                    (light.lux >= 400.0f) ||   // too bright
                    (light.lux <= 300.0f);     // too dim
}
