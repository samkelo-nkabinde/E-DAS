/*
 * date.c
 *
 *  Created on: Apr 17, 2026
 *      Author: Samkelo Nkabinde
 */
#include "date.h"

Date_t system_date;

static uint8_t get_weekday(uint32_t year, uint32_t month, uint32_t day)
{

    static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (month < 3) year--;
    uint8_t dow = (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;

    return (dow == 0) ? RTC_WEEKDAY_SUNDAY : dow;
}

void date_init(void)
{

	system_date.year    = 2026;
	system_date.month   = 2;
	system_date.day     = 26;
	system_date.hours   = 22;
	system_date.mins    = 12;
	system_date.seconds = 42;
}

void date_set(uint32_t year, uint32_t month, uint32_t day,
              uint32_t hours, uint32_t mins, uint32_t seconds)
{

    if (year  < 2000 || year  > 2099) return;
    if (month < 1    || month > 12  ) return;
    if (day   < 1    || day   > 31  ) return;
    if (hours > 23 || mins > 59 || seconds > 59) return;

    system_date.year    = year;
    system_date.month   = month;
    system_date.day     = day;
    system_date.hours   = hours;
    system_date.mins    = mins;
    system_date.seconds = seconds;

    RTC_TimeTypeDef newTime = {0};
    newTime.Hours          = (uint8_t)hours;
    newTime.Minutes        = (uint8_t)mins;
    newTime.Seconds        = (uint8_t)seconds;
    newTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    newTime.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_RTC_SetTime(&hrtc, &newTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
        return;
    }


    RTC_DateTypeDef newDate = {0};
    newDate.Year    = (uint8_t)(year - 2000);
    newDate.Month   = (uint8_t)month;
    newDate.Date    = (uint8_t)day;
    newDate.WeekDay = get_weekday(year, month, day);

    if (HAL_RTC_SetDate(&hrtc, &newDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
}

void date_update(Date_t *date)
{
    RTC_DateTypeDef sDate = {0};
    RTC_TimeTypeDef sTime = {0};
    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    date->year    = 2000 + sDate.Year;
    date->month   = sDate.Month;
    date->day     = sDate.Date;
    date->hours   = sTime.Hours;
    date->mins    = sTime.Minutes;
    date->seconds = sTime.Seconds;
}

void date_format(Date_t *date, char *buf, size_t buf_size)
{
    snprintf(buf, buf_size, "%04lu/%02lu/%02lu %02lu:%02lu:%02lu",
             date->year,
             date->month,
             date->day,
             date->hours,
             date->mins,
             date->seconds);
}
