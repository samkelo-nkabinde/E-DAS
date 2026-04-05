/*
 * keypad.h
 *
 *  Created on: Apr 4, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

#include "main.h"
#include <stdint.h>
#define ROW_NUM 4
#define COL_NUM 3
#define DEBOUNCE_MS 60

typedef struct {
    GPIO_TypeDef* Port;
    uint16_t Pin;
    uint8_t last_state;
    uint8_t last_reading;
    uint32_t last_debounce;
} keypad_row;

typedef struct {
    GPIO_TypeDef *Port;
    uint16_t Pin;
} keypad_t;

extern keypad_row row_pins[ROW_NUM];
extern keypad_row col_pins[COL_NUM];

extern char keypad_map[ROW_NUM][COL_NUM];

void keypad_init(void);
char keypad_get_key(void);

#endif /* INC_KEYPAD_H_ */
