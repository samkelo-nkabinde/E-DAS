/*
 * button.c
 *
 *  Created on: Feb 25, 2026
 *      Author: Samkelo Nkabinde
 */

#include "button.h"

Button_t S1;
Button_t S2;
Button_t S3;
Button_t S4;
Button_t S5;

void button_init(void)
{
	Button_t S1 = {GPIOC, GPIO_PIN_1, 0, GPIO_PIN_RESET, GPIO_PIN_RESET};
	Button_t S2 = {GPIOC, GPIO_PIN_2, 0, GPIO_PIN_RESET, GPIO_PIN_RESET};
	Button_t S3 = {GPIOC, GPIO_PIN_0, 0, GPIO_PIN_RESET, GPIO_PIN_RESET};
	Button_t S4 = {GPIOB, GPIO_PIN_6, 0, GPIO_PIN_RESET, GPIO_PIN_RESET};
	Button_t S5 = {GPIOB, GPIO_PIN_0, 0, GPIO_PIN_RESET, GPIO_PIN_RESET};
	return;
}

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
