/*
 * oled.h
 *
 *  Created on: Apr 11, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "ssd1306.h"
#include "ssd1306_fonts.h"

#define FONT Font_7x10

void OLED_init();
void OLED_write(char *string, int32_t x_pos, int32_t y_pos);
void OLED_clear();

#endif /* INC_OLED_H_ */
