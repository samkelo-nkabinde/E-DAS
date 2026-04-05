/*
 * keypad.c
 *
 *  Created on: Apr 4, 2026
 *      Author: Samkelo Nkabinde
 */

#include "keypad.h"

keypad_row row_pins[ROW_NUM] = {
    {GPIOB, GPIO_PIN_1, 0, 0, 0},
    {GPIOA, GPIO_PIN_12,0, 0, 0},
    {GPIOA, GPIO_PIN_11,0, 0, 0},
    {GPIOB, GPIO_PIN_2, 0, 0, 0}
};

keypad_row col_pins[COL_NUM] = {
    {GPIOB, GPIO_PIN_13},
    {GPIOB, GPIO_PIN_14},
    {GPIOB, GPIO_PIN_15}
};

char keypad_map[ROW_NUM][COL_NUM] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

void keypad_init(void)
{
    for(int i=0; i<ROW_NUM; i++)
        HAL_GPIO_WritePin(row_pins[i].Port, row_pins[i].Pin, GPIO_PIN_SET);
}

char keypad_get_key(void)
{
    for(int r=0; r<ROW_NUM; r++)
    {
        for(int i=0; i<ROW_NUM; i++)
            HAL_GPIO_WritePin(row_pins[i].Port, row_pins[i].Pin, GPIO_PIN_SET);

        HAL_GPIO_WritePin(row_pins[r].Port, row_pins[r].Pin, GPIO_PIN_RESET);

        for(int c=0; c<COL_NUM; c++)
        {
            uint8_t reading = HAL_GPIO_ReadPin(col_pins[c].Port, col_pins[c].Pin);

            if(reading != row_pins[r].last_reading)
                row_pins[r].last_debounce = HAL_GetTick();

            row_pins[r].last_reading = reading;

            if((HAL_GetTick() - row_pins[r].last_debounce) > DEBOUNCE_MS)
            {
                if(reading != row_pins[r].last_state)
                {
                    row_pins[r].last_state = reading;

                    if(reading == GPIO_PIN_RESET)
                        return keypad_map[r][c];
                }
            }
        }
    }
    return 0;
}
