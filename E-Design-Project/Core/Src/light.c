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
    // 1. Wake up the ADC and ask it for exactly ONE reading
    HAL_ADC_Start(&hadc1);

    // 2. Wait up to 10ms for that single reading to finish
    if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
    {
        light.raw = (uint16_t)HAL_ADC_GetValue(&hadc1);

        // Filter the raw integer signal directly (cast to float for Kalman)
        float raw_filtered = kalman_update(&kf_light, (float)light.raw);

        // Calculate voltage if you still need it for telemetry/other logic
        light.voltage = (VREF * raw_filtered) / ADC_MAX;

        // --- NEW CALIBRATED MATH ---
        // Maps Raw 216 -> 300 Lux | Raw 822 -> 400 Lux
        light.lux = (0.165f * raw_filtered) + 264.36f;

        // Prevent negative Lux values in very dark rooms
        if (light.lux < 0.0f)
        {
            light.lux = 0.0f;
        }
    }

    // 3. Cleanly shut down the ADC
    HAL_ADC_Stop(&hadc1);

    light.warning = light_external_warning || (light.lux < 300); // Note: lux < 0 will always be false here due to the check above
}
