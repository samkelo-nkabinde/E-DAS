/*
 * oled.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Samkelo Nkabinde
 */

#include "oled.h"

void OLED_init(void)
{
    ssd1306_Init();
    ssd1306_WriteCommand(0xA0);
    ssd1306_WriteCommand(0xC0);
    OLED_clear();
    OLED_update();
}

void OLED_write(char *string, int32_t x_pos, int32_t y_pos)
{
    ssd1306_SetCursor(x_pos, y_pos);
    ssd1306_WriteString(string, FONT, White);
}

void OLED_clear(void)
{
    ssd1306_Fill(Black);
}

void OLED_update(void)
{
    ssd1306_UpdateScreen();
}
