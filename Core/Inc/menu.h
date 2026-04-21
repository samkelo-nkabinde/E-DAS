/*
 * menu.h
 *
 *  Created on: Apr 19, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "oled.h"
#include "temperature.h"
#include "distance.h"
#include "light.h"
#include "fuel.h"
#include "date.h"

void display_default_page(void);
void display_entry_page(void);
void display_diagnostics_page(void);

void display_1_1(void);
void display_1_2(void);
void display_1_3(void);
void display_1_4(void);
void display_1_5(void);

void display_2_1(uint8_t editing);
void display_2_2(uint8_t editing);
void display_2_3(uint8_t logging_enabled);
void display_3_1(uint8_t sd_ok);
void display_3_2(uint8_t mpu_ok);
void display_3_3(uint8_t gps_ok);
void display_3_4(uint8_t logging_enabled);

void display_warn_unsafe_driving(void);
void display_warn_impact(void);
void display_warn_low_light(void);
void display_warn_proximity(void);
void display_warn_high_temp(void);

#endif /* INC_MENU_H_ */
