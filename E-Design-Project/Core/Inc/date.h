/*
 * date.h
 *
 *  Created on: Apr 17, 2026
 *      Author: Samkelo Nkabinde
 */

#ifndef INC_DATE_H_
#define INC_DATE_H_

#include "main.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

extern RTC_DateTypeDef sDate;
extern RTC_TimeTypeDef sTime;

typedef struct
{
	uint32_t year;
	uint32_t month;
	uint32_t day;
	uint32_t hours;
	uint32_t mins;
	uint32_t seconds;
}Date_t;

extern Date_t system_date;

void date_init(void);
void date_set(uint32_t year, uint32_t month, uint32_t day,
              uint32_t hours, uint32_t mins, uint32_t seconds);
void date_update(Date_t *date);
void date_format(Date_t *date, char *buf, size_t buf_size, uint8_t type);

#endif /* INC_DATE_H_ */
