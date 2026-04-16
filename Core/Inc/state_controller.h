/*
 * state_controller.h
 *
 *  Created on: Apr 16, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_STATE_CONTROLLER_H_
#define INC_STATE_CONTROLLER_H_

#include "main.h"
#include "button.h"
#include "oled.h"

typedef enum {
    MENU_HOME = 0,
    MENU_DATA_ENTRY,
    MENU_MEASUREMENTS,
    MENU_DIAGNOSTICS,
    MENU_COUNT
} menu_option;

typedef enum {
    MODE_HOME,
    MODE_LEVEL1,
    MODE_LEVEL2,
	MODE_LEVEL3,
	MODE_LEVEL4,
	MODE_LEVEL5,
    MODE_DATA_ENTRY
} menu_mode;


typedef enum {
    WARN_NONE       = 0,
    WARN_UNSAFE_DRV = 1,
    WARN_LOW_LIGHT  = 2,
    WARN_PROXIMITY  = 3,
    WARN_HIGH_TEMP  = 4,
} Warning_t;



#endif /* INC_STATE_CONTROLLER_H_ */
