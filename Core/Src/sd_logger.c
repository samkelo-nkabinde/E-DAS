/*
 * sd_logger.c
 *
 *  Created on: May 14, 2026
 *      Author: Samkelo Nkabinde
 */

#include "sd_logger.h"

static FATFS fs;
static FIL log_file;

#define SD_LOG_FILE_NAME "edas.csv"

SD_LogEntry_t entry;
bool log_data = false;
bool sd_card_ready = false;

static void sd_format_date_time(const Date_t *date, char *buffer, size_t size)
{
    snprintf(
        buffer,
        size,
        "%04lu/%02lu/%02lu %02lu:%02lu:%02lu",
        date->year,
        date->month,
        date->day,
        date->hours,
        date->mins,
        date->seconds
    );
}

uint8_t SD_Logger_Init(void)
{
    FRESULT result;


    result = f_mount(&fs, "", 1);


    if (result != FR_OK)
        return 0;

    result = f_open(&log_file, SD_LOG_FILE_NAME, FA_OPEN_ALWAYS | FA_WRITE);

    if (result != FR_OK)
        return 0;

    f_close(&log_file);

    return 1;
}

uint8_t SD_Logger_WriteEntry(const SD_LogEntry_t *entry)
{
    FRESULT result;
    char line[180];
    char date_time[24];
    UINT bw;

    sd_format_date_time(&entry->date, date_time, sizeof(date_time));

    result = f_open(&log_file, SD_LOG_FILE_NAME, FA_OPEN_APPEND | FA_WRITE);

    if (result != FR_OK)
        return 0;

    snprintf(
        line,
        sizeof(line),
        "%s,%04u,%.1f,%04.1f,%.2f,%.2f,%.2f,%u,%u,%u,%u,%u,%.6f,%.6f\r\n",
        date_time,
        entry->light_lux,
        entry->temperature_c,
        entry->distance_cm,
        entry->accel_x_g,
        entry->accel_y_g,
        entry->accel_z_g,
        entry->unsafe_driving,
        entry->impact_warning,
        entry->low_light_warning,
        entry->proximity_warning,
        entry->high_temperature_warning,
        entry->latitude,
        entry->longitude
    );

    result = f_write(&log_file, line, strlen(line), &bw);

    if (result != FR_OK || bw != strlen(line))
    {
        f_close(&log_file);
        return 0;
    }

    f_sync(&log_file);
    f_close(&log_file);

    return 1;
}

uint8_t SD_Logger_PrintFileUART(void)
{
    FRESULT result;
    FIL file;
    char line[200];

    result = f_open(&file, SD_LOG_FILE_NAME, FA_READ);

    if (result != FR_OK)
        return 0;

    while (f_gets(line, sizeof(line), &file) != NULL)
    {
        UART_transmit(&g_uart2, (uint8_t *)line, strlen(line));
    }

    f_close(&file);
    return 1;
}

uint8_t SD_Logger_ClearFile(void)
{
    FRESULT result;
    FIL file;

    result = f_open(&file, SD_LOG_FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE);

    if (result != FR_OK)
        return 0;

    f_sync(&file);
    f_close(&file);

    return 1;
}

