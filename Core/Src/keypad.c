/*
 * keypad.c
 *
 *  Created on: Apr 4, 2026
 *      Author: Samkelo Nkabinde
 */

#include "keypad.h"


keypad_pin row_pins[ROW_NUM] = {
    {GPIOB, GPIO_PIN_1},
    {GPIOA, GPIO_PIN_12},
    {GPIOA, GPIO_PIN_11},
    {GPIOB, GPIO_PIN_2}
};


keypad_pin col_pins[COL_NUM] = {
    {GPIOB, GPIO_PIN_13},
    {GPIOB, GPIO_PIN_14},
    {GPIOB, GPIO_PIN_15}
};

volatile uint8_t keypad_event = 0;


uint8_t keypad_map[ROW_NUM][COL_NUM] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

void keypad_init(void)
{
	keypad_reset();
}

void keypad_reset(void)
{
    for(int i = 0; i < ROW_NUM; i++)
        HAL_GPIO_WritePin(row_pins[i].Port, row_pins[i].Pin, GPIO_PIN_RESET);

    return;
}

uint8_t keypad_get_key(void)
{
    char detected_key = 0;


    HAL_Delay(30);


    for(int r = 0; r < ROW_NUM; r++)
    {

        for(int i = 0; i < ROW_NUM; i++)
        {
            HAL_GPIO_WritePin(row_pins[i].Port, row_pins[i].Pin, GPIO_PIN_SET);
        }


        HAL_GPIO_WritePin(row_pins[r].Port, row_pins[r].Pin, GPIO_PIN_RESET);


        for(volatile int d = 0; d < 1000; d++);


        for(int c = 0; c < COL_NUM; c++)
        {
            if(HAL_GPIO_ReadPin(col_pins[c].Port, col_pins[c].Pin) == GPIO_PIN_RESET)
            {
                detected_key = keypad_map[r][c];


                while(HAL_GPIO_ReadPin(col_pins[c].Port, col_pins[c].Pin) == GPIO_PIN_RESET);

                break;
            }
        }
        if (detected_key) break;
    }

    return detected_key;
}

void keypad_clear(void)
{
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
	return;
}
