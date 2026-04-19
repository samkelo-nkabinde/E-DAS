/*
 * light.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_LIGHT_H_
#define INC_LIGHT_H_

#include "main.h"

#include <stdbool.h>

#include "kalman_filter.h"

#define VREF        3.3f
#define ADC_MAX     4095.0f
#define CALIBRATION_CONSTANT 800.0f;

typedef struct
{
    uint16_t raw;       // ADC value (0–4095)
    float voltage;      // Converted voltage (V)
    float filtered;     // Smoothed signal (V)
    float lux;          // Calibrated light intensity
    bool warning;

} light_t;

extern light_t light;
extern bool light_external_warning;

void light_init();
void light_update();

#endif /* INC_LIGHT_H_ */
