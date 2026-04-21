/*
 * state_machine.h
 *
 * Created on: Apr 17, 2026
 * Author: Samkelo Nkabinde
 */

#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"
#include "button.h"
#include "oled.h"
#include "menu.h"
#include "keypad.h"
#include "light.h"
#include "temperature.h"
#include "distance.h"
#include "date.h"

typedef enum {
    STATE_TOP_MENU,
    STATE_MEASUREMENTS,
    STATE_ENTRY,
    STATE_DIAGNOSTICS,
    STATE_WARNING
} state_e;

typedef enum {
    TOP_MEASUREMENTS,
    TOP_ENTRY,
    TOP_DIAGNOSTICS
} top_menu_e;

typedef enum {
    PAGE_1_1,
    PAGE_1_2,
    PAGE_1_3,
    PAGE_1_4,
    PAGE_1_5
} measurements_page_e;

typedef enum {
    PAGE_2_1 = 1,
    PAGE_2_2,
    PAGE_2_3
} entry_page_e;

typedef enum {
    PAGE_3_1,
    PAGE_3_2,
    PAGE_3_3,
    PAGE_3_4
} diagnostics_page_e;

extern state_e current_state;
extern top_menu_e current_top_menu;
extern measurements_page_e m_page;
extern entry_page_e e_page;
extern diagnostics_page_e d_page;

void state_machine(void);

#endif /* INC_STATE_MACHINE_H_ */
