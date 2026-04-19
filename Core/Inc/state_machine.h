///*
// * state_machine.h
// *
// *  Created on: Apr 17, 2026
// *      Author: Samkelo Nkabinde
// */
//
//#ifndef INC_STATE_MACHINE_H_
//#define INC_STATE_MACHINE_H_
//
//#include <string.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include "main.h"
//#include "button.h"
//#include "oled.h"
//#include "command.h"
//
//#define MAX_OLED_CHAR     19
//#define PAGE_COUNT         3
//#define DEFAULT_SUBPAGE_COUNT       5
//#define DATA_ENTRY_SUBPAGE_COUNT    3
//#define DIAGNOSTICS_SUBPAGE_COUNT   4
//
//typedef enum
//{
//    WARN_NONE      = 0x00,
//    WARN_UNSAFE    = 0x01,
//    WARN_LOW_LIGHT = 0x02,
//    WARN_PROX      = 0x04,
//    WARN_HIGH_TEMP = 0x08,
//} WarningFlag;
//
//typedef enum {
//    NOT_EDITING = 0,
//    EDITING
//} SubPage_state;
//
//typedef enum
//{
//    NOT_DISPLAYED = 0,
//    DISPLAYED
//} PageState;
//
//typedef enum
//{
//    DEFAULT     = 0,
//    DATA_ENTRY,
//    DIAGNOSTICS
//} PageType;
//
//typedef enum
//{
//    LEVEL_TOP    = 0,
//    LEVEL_SUB
//} NavLevel;
//
//typedef struct
//{
//    char date[MAX_OLED_CHAR];
//    char quantity_1[MAX_OLED_CHAR];
//    char quantity_2[MAX_OLED_CHAR];
//    SubPage_state state;
//} SubPage_t;
//
//typedef struct
//{
//    char date[MAX_OLED_CHAR];
//    char title[MAX_OLED_CHAR];
//    char instruction[MAX_OLED_CHAR];
//    PageState state;
//    SubPage_t *subpage_list;
//    uint8_t   subpage_count;
//} Page_t;
//
//extern Page_t   page_list[PAGE_COUNT];
//extern uint8_t  page_index;
//extern uint8_t  subpage_index;
//extern NavLevel nav_level;
//extern uint8_t  active_warning;
//
//extern SubPage_t measurements_subpages[DEFAULT_SUBPAGE_COUNT];
//extern SubPage_t data_entry_subpages[DATA_ENTRY_SUBPAGE_COUNT];
//extern SubPage_t diagnostics_subpages[DIAGNOSTICS_SUBPAGE_COUNT];
//
//void SM_init(void);
//void SM_tick(void);
//
//
//void set_page(Page_t *page, const char *title,
//              const char *instruction, PageState state,
//              SubPage_t *subpages, uint8_t subpage_count);
//
//void display_page(Page_t *page);
//void display_subpage(SubPage_t *subpage);
//
//
//void SM_check_warnings(void);
//void SM_display_warning(WarningFlag w);
//
//#endif /* INC_STATE_MACHINE_H_ */
