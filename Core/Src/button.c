/*
 * button.c
 *
 *  Created on: Feb 25, 2026
 *      Author: Samkelo Nkabinde
 */

#include "button.h"

uint8_t button_pressed(Button_t *button)
{
    uint8_t reading = HAL_GPIO_ReadPin(button->port, button->pin);

    if (reading != button->last_reading)
    {
        button->last_debounce = HAL_GetTick();
    }

    button->last_reading = reading;

    if ((HAL_GetTick() - button->last_debounce) > 50)
    {
        if (reading != button->last_state)
        {
            button->last_state = reading;

            if (reading == GPIO_PIN_SET)
            {
                return 1;
            }
        }
    }

    return 0;
}
