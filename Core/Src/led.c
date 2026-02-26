/*
 * led.c
 *
 *  Created on: Feb 25, 2026
 *      Author: Samkelo Nkabinde
 */

#include "led.h"


void LED_on(LED_t *led)
{
	led->state = 1;
	HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}
void LED_off(LED_t *led)
{
	led->state = 0;
	HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

void LED_toggle(LED_t *led)
{
	led->state = !led->state;
	HAL_GPIO_WritePin(led->port, led->pin, led->state);
}

void LED_blink_control(LED_t *led)
{
    if (!led->blink_enable)
        return;

    if (HAL_GetTick() - led->blink_timer >= 500)
    {
        led->blink_timer = HAL_GetTick();
        LED_toggle(led);
    }
}

void LED_update(LED_t *led)
{
    if (!led->blink_enable)
    {
        HAL_GPIO_WritePin(led->port, led->pin, led->state);
    }
    else
    {
        if (HAL_GetTick() - led->blink_timer >= 500)
        {
            led->blink_timer = HAL_GetTick();
            HAL_GPIO_TogglePin(led->port, led->pin);
        }
    }
}
