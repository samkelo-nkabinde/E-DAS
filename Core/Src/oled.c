/*
 * oled.c
 *
 *  Created on: Apr 11, 2026
 *      Author: Samkelo Nkabinde
 */

#include "oled.h"

void OLED_init()
{
	  ssd1306_Init();
	  ssd1306_WriteCommand(0xA0);
	  ssd1306_WriteCommand(0xC0);
	  ssd1306_Fill(Black);

	  return;
}

void OLED_write(char *string, int32_t x_pos, int32_t y_pos)
{
	ssd1306_SetCursor(x_pos, y_pos);

	ssd1306_WriteString(string, FONT, White);

	ssd1306_UpdateScreen();
	return;
}

void OLED_clear()
{
	ssd1306_Fill(Black);
	return;
}
