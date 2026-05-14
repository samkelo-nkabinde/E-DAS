/*
 * sd_logger.h
 *
 *  Created on: May 14, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_SD_LOGGER_H_
#define INC_SD_LOGGER_H_

#include "fatfs.h"
#include "date.h"
#include "uart_handle.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    Date_t date;

    uint16_t light_lux;
    float temperature_c;
    float distance_cm;

    float accel_x_g;
    float accel_y_g;
    float accel_z_g;

    uint8_t unsafe_driving;
    uint8_t impact_warning;
    uint8_t low_light_warning;
    uint8_t proximity_warning;
    uint8_t high_temperature_warning;

    float latitude;
    float longitude;

} SD_LogEntry_t;

extern SD_LogEntry_t entry;
extern bool log_data;
extern bool sd_card_ready;

uint8_t SD_Logger_PrintFileUART(void);
uint8_t SD_Logger_Init(void);
uint8_t SD_Logger_WriteEntry(const SD_LogEntry_t *entry);
uint8_t SD_Logger_ClearFile(void);
#endif /* INC_SD_LOGGER_H_ */
