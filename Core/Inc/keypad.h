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
} keypad_pin;


extern keypad_pin row_pins[ROW_NUM];
extern keypad_pin col_pins[COL_NUM];

extern uint8_t keypad_map[ROW_NUM][COL_NUM];
extern volatile uint8_t keypad_event;

void keypad_init(void);
void keypad_reset(void);
uint8_t keypad_get_key(void);
void keypad_clear(void);

#endif /* INC_KEYPAD_H_ */
