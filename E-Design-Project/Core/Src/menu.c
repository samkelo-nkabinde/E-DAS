/*
 * menu.c
 *
 *  Created on: Apr 19, 2026
 *      Author: Samkelo Nkabinde
 */

#include "menu.h"

static void format_line(char out[19],
                      const char *label,
                      const char *val_fmt, ...)
{
    char val[32];
    va_list args;
    va_start(args, val_fmt);
    vsnprintf(val, sizeof(val), val_fmt, args);
    va_end(args);

    int llen = (int)strlen(label);
    int vlen = (int)strlen(val);
    int pad  = 18 - llen - vlen;
    if (pad < 0) pad = 0;

    int pos = 0;
    for (int i = 0; i < llen && pos < 18; i++) out[pos++] = label[i];
    for (int i = 0; i < pad  && pos < 18; i++) out[pos++] = ' ';
    for (int i = 0; i < vlen && pos < 18; i++) out[pos++] = val[i];

    out[18] = '\0';
}

void display_default_page(void)
{
	OLED_clear();
	char date[20];
	date_format(&system_date, date, sizeof(date), 1);
	OLED_write(date, 0, 0);
	OLED_write("== Measurements ==", 0, 12);
	OLED_write("Press-> display", 0, 22);
	OLED_update();
}

void display_entry_page(void)
{
	OLED_clear();
	char date[20];
	date_format(&system_date, date, sizeof(date), 1);
	OLED_write(date, 0, 0);
    OLED_write("=== Data Entry ===", 0, 12);
    OLED_write("Press-> display",   0, 22);
    OLED_update();
}

void display_diagnostics_page(void)
{
	OLED_clear();
	char date[20];
	date_format(&system_date, date, sizeof(date), 1);
	OLED_write(date, 0, 0);
    OLED_write("== Diagnostics ===", 0, 12);
    OLED_write("Press-> display",   0, 22);
    OLED_update();
}


void display_1_1(void)
{
	OLED_clear();
    char date[20];
    char line2[19];
    char line3[19];


    format_line(line2, "Dist:", "%.1f cm",  distance.filtered);
    format_line(line3, "Temp:", "%.1f C",   temperature.filtered);
	date_format(&system_date, date, sizeof(date), 1);
	OLED_write(date, 0, 0);
    OLED_write(line2, 0, 12);
    OLED_write(line3, 0, 22);
	OLED_update();
}

void display_1_2(void)
{
	OLED_clear();
    char date[20];
    char line2[20];
    char line3[20];

    format_line(line2, "Accel:", "|%.2f| g", 0.0f);
    format_line(line3, "Light:", "%d lux",  (int)light.lux);
    date_format(&system_date, date, sizeof(date), 1);

    OLED_write(date,  0, 0);
    OLED_write(line2, 0, 11);
    OLED_write(line3, 0, 22);
    OLED_update();
}

void display_1_3(void)
{
	OLED_clear();
    char date[20];
    char line2[20];
    char line3[20];
    date_format(&system_date, date, sizeof(date), 1);

    format_line(line2, "Lat:", "%.6f",  0.0f);
    format_line(line3, "Long:", "%.6f", 0.0f);
    OLED_write(date,  0, 0);
    OLED_write(line2, 0, 12);
    OLED_write(line3, 0, 22);
    OLED_update();
}

void display_1_4(void)
{
	OLED_clear();
    char date[20];
    char line2[20];
    char line3[20];
    date_format(&system_date, date, sizeof(date), 1);

    format_line(line2, "Heading:", "%d deg",  0);
    format_line(line3, "Speed:", "%.1f km/h", 0);
    OLED_write(date,  0, 0);
    OLED_write(line2, 0, 12);
    OLED_write(line3, 0, 22);
    OLED_update();
}

void display_1_5(void)
{
	OLED_clear();
    char line2[20];
    char line3[20];
    format_line(line2,"", "%.1f km/L", km_per_l);
    format_line(line3,"", "%.1f L/100 km", l_per_100km);
    OLED_write("Fuel Efficiency:", 0, 0);
    OLED_write(line2, 0, 12);
    OLED_write(line3, 0, 22);
    OLED_update();
}


void display_2_1(uint8_t editing, uint32_t preview)
{
	OLED_clear();
    char line2[20];
    if (editing)
    	format_line(line2, "Current:", "%d L", (int)preview);
    else
    	format_line(line2, "Current:", "%.1f L", last_liters);
    OLED_write("Enter fuel liters", 0, 0);
    OLED_write(line2, 0, 12);
    OLED_write(editing ? "Press S3 to accept"
                       : "Press S3 to change", 0, 22);
    OLED_update();
}

void display_2_2(uint8_t editing, uint32_t preview)
{
	OLED_clear();
    char line2[20];
    if (editing)
    	format_line(line2, "Current:", "%d km", (int)preview);
    else
    	format_line(line2, "Current:", "%.1f km", last_distance);
    OLED_write("Enter odometer km", 0, 0);
    OLED_write(line2, 0, 12);
    OLED_write(editing ? "Press S3 to accept"
                       : "Press S3 to change", 0, 22);
    OLED_update();
}

void display_2_3(uint8_t logging_enabled)
{
	OLED_clear();
    OLED_write("Log data (Y/N)", 0, 0);
    OLED_write("'*' = Y / '#' = N",0, 12);
    OLED_write(logging_enabled ? "Log Data: ENABLED"
                               : "Log Data: DISABLED", 0, 22);
    OLED_update();
}


void display_3_1(uint8_t sd_ok)
{
	OLED_clear();
    char date[20];
    date_format(&system_date, date, sizeof(date), 1);
    OLED_write(date, 0, 0);
    OLED_write("-- Diagnostics ---",0, 11);
    OLED_write(sd_ok ? "SD-card: OK"
                     : "SD-card: NOT OK",0, 22);
    date_update(&system_date);
    OLED_update();
}

void display_3_2(uint8_t mpu_ok)
{
	OLED_clear();
    char date[20];
    date_format(&system_date, date, sizeof(date), 1);
    OLED_write(date, 0, 0);
    OLED_write("-- Diagnostics ---", 0, 11);
    OLED_write(mpu_ok ? "MPU-6050: OK"
                      : "MPU-6050: NOT OK", 0, 22);
    date_update(&system_date);
    OLED_update();
}

void display_3_3(uint8_t gps_ok)
{
	OLED_clear();
    char date[20];
    date_format(&system_date, date, sizeof(date), 1);
    OLED_write(date, 0, 0);
    OLED_write("-- Diagnostics ---", 0, 11);
    OLED_write(gps_ok ? "GPS: OK"
                      : "GPS: NOT OK", 0, 22);
    date_update(&system_date);
    OLED_update();
}

void display_3_4(uint8_t logging_enabled)
{
	OLED_clear();
    char date[20];
    date_format(&system_date, date, sizeof(date), 1);
    OLED_write(date, 0, 0);
    OLED_write("-- Diagnostics ---", 0, 11);
    OLED_write(logging_enabled ? "Log Data: ENABLED"
                               : "Log Data: DISABLED", 0, 22);
    date_update(&system_date);
    OLED_update();
}


void display_warn_unsafe_driving(void)
{
	OLED_clear();
    char line3[19];
    format_line(line3, "Accel:", "|%.2f| g", 0.0f);
    OLED_write("===== WARNING ====", 0, 0);
    OLED_write("--Unsafe Driving--", 0, 11);
    OLED_write(line3, 0, 22);
    OLED_update();
}

void display_warn_impact(void)
{
	OLED_clear();
    char line3[19];
    format_line(line3, "Accel:", "|%.2f| g", 0.0f);
    OLED_write("===== WARNING ====", 0, 0);
    OLED_write("-Impact Detected--", 0, 11);
    OLED_write(line3, 0, 22);
    OLED_update();
}

void display_warn_low_light(void)
{
	OLED_clear();
    char line3[19];
    format_line(line3, "Light:", "%d lux", (int)light.lux);
    OLED_write("===== WARNING ====", 0, 0);
    OLED_write("----Low Light-----", 0, 11);
    OLED_write(line3, 0, 22);
    OLED_update();
}

void display_warn_proximity(void)
{
	OLED_clear();
    char line3[19];
    format_line(line3, "Dist:", "%.1f cm", distance.filtered);
    OLED_write("===== WARNING ====", 0, 0);
    OLED_write("----Proximity-----", 0, 11);
    OLED_write(line3, 0, 22);
    OLED_update();
}

void display_warn_high_temp(void)
{
	OLED_clear();
    char line3[19];
    format_line(line3, "Temp:", "%.1f C", temperature.filtered);
    OLED_write("===== WARNING ====", 0, 0);
    OLED_write("-High Temperature-", 0, 11);
    OLED_write(line3,0, 22);
    OLED_update();
}
