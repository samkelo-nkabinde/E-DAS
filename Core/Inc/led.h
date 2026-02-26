/*
 * led.h
 *
 *  Created on: Feb 25, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include <stdint.h>
#include "main.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t state;          // ON or OFF
    uint8_t blink_enable;    // 1 = blinking active
    uint32_t blink_timer;    // last toggle time
} LED_t;

void LED_on(LED_t *led);
void LED_off(LED_t *led);
void LED_toggle(LED_t *led);
void LED_blink_control(LED_t *led);
void LED_update(LED_t *led);

#endif /* INC_LED_H_ */
