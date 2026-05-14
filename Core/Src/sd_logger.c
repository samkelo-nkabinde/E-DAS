/*
 * sd_logger.c
 *
 *  Created on: May 14, 2026
 *      Author: Samkelo Nkabinde
 */

#include "sd_logger.h"

static FATFS fs;
static FIL log_file;
static UINT bytes_written;

#define SD_LOG_FILE_NAME "edas.csv"

SD_LogEntry_t entry;
bool log_data = false;

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
    char msg[100];

    result = f_mount(&fs, "", 1);

    snprintf(msg, sizeof(msg), "f_mount result: %d\r\n", result);
    UART_transmit(&g_uart2, (uint8_t *)msg, strlen(msg));

    if (result != FR_OK)
    {
        return 0;
    }

    result = f_open(&log_file, SD_LOG_FILE_NAME, FA_OPEN_ALWAYS | FA_WRITE);

    snprintf(msg, sizeof(msg), "f_open result: %d\r\n", result);
    UART_transmit(&g_uart2, (uint8_t *)msg, strlen(msg));

    if (result != FR_OK)
    {
        return 0;
    }

    if (f_size(&log_file) == 0)
    {
        const char *header =
            "DateTime,Light_lux,Temperature_C,Distance_cm,"
            "Accel_X_g,Accel_Y_g,Accel_Z_g,"
            "UnsafeDriving,ImpactWarning,LowLightWarning,"
            "ProximityWarning,HighTemperature,"
            "Latitude,Longitude\r\n";

        UINT bytes_written;
        result = f_write(&log_file, header, strlen(header), &bytes_written);

        snprintf(msg, sizeof(msg), "header write result: %d\r\n", result);
        UART_transmit(&g_uart2, (uint8_t *)msg, strlen(msg));

        if (result != FR_OK)
        {
            f_close(&log_file);
            return 0;
        }
    }

    f_close(&log_file);

    return 1;
}

uint8_t SD_Logger_WriteEntry(const SD_LogEntry_t *entry)
{
    FRESULT result;
    char line[180];
    char date_time[24];

    sd_format_date_time(&entry->date, date_time, sizeof(date_time));

    result = f_open(&log_file, SD_LOG_FILE_NAME, FA_OPEN_APPEND | FA_WRITE);

    if (result != FR_OK)
    {
        return 0;
    }

    snprintf(
        line,
        sizeof(line),
        "%s,%04u,%+.1f,%04.1f,%+.2f,%+.2f,%+.2f,%u,%u,%u,%u,%u,%+.6f,%+.6f\r\n",
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

    result = f_write(&log_file, line, strlen(line), &bytes_written);

    if (result != FR_OK || bytes_written != strlen(line))
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
    char msg[80];

    result = f_open(&file, SD_LOG_FILE_NAME, FA_READ);

    if (result != FR_OK)
    {
        snprintf(
            msg,
            sizeof(msg),
            "SD read failed: could not open %s, error code: %d\r\n",
            SD_LOG_FILE_NAME,
            result
        );

        UART_transmit(&g_uart2, (uint8_t *)msg, strlen(msg));
        return 0;
    }

    UART_transmit(&g_uart2, (uint8_t *)"\r\n--- SD Card Log File ---\r\n",
                  strlen("\r\n--- SD Card Log File ---\r\n"));

    while (f_gets(line, sizeof(line), &file) != NULL)
    {
        UART_transmit(&g_uart2, (uint8_t *)line, strlen(line));
    }

    UART_transmit(&g_uart2, (uint8_t *)"--- End of SD Card Log File ---\r\n",
                  strlen("--- End of SD Card Log File ---\r\n"));

    f_close(&file);
    return 1;
}

uint8_t SD_Logger_ClearFile(void)
{
    FRESULT result;
    FIL file;
    UINT bytes_written;

    const char *header =
        "DateTime,Light_lux,Temperature_C,Distance_cm,"
        "Accel_X_g,Accel_Y_g,Accel_Z_g,"
        "UnsafeDriving,ImpactWarning,LowLightWarning,"
        "ProximityWarning,HighTemperature,"
        "Latitude,Longitude\r\n";

    /*
     * FA_CREATE_ALWAYS clears the file if it already exists.
     * If the file does not exist, it creates it.
     */
    result = f_open(&file, SD_LOG_FILE_NAME, FA_CREATE_ALWAYS | FA_WRITE);

    if (result != FR_OK)
    {
        return 0;
    }

    result = f_write(&file, header, strlen(header), &bytes_written);

    if (result != FR_OK || bytes_written != strlen(header))
    {
        f_close(&file);
        return 0;
    }

    f_sync(&file);
    f_close(&file);

    return 1;
}
