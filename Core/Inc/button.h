/*
 * button.h
 *
 *  Created on: Feb 25, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include <stdint.h>
#include "main.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t last_debounce;
    uint8_t last_state;
    uint8_t last_reading;
} Button_t;

extern Button_t S1;
extern Button_t S2;
extern Button_t S3;
extern Button_t S4;
extern Button_t S5;

void button_init(void);
uint8_t button_pressed(Button_t *button);

#endif /* INC_BUTTON_H_ */
