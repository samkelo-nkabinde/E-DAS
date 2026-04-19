/*
 * stats.c
 *
 *  Created on: Mar 11, 2026
 *      Author: Samkelo Nkabinde
 */

#include "stats.h"

static void stat_line(char out[22],
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
    int pad  = 20 - llen - vlen;
    if (pad < 0) pad = 0;

    int pos = 0;
    for (int i = 0; i < llen && pos < 20; i++) out[pos++] = label[i];
    for (int i = 0; i < pad  && pos < 20; i++) out[pos++] = ' ';
    for (int i = 0; i < vlen && pos < 20; i++) out[pos++] = val[i];

    out[20] = '\n';
    out[21] = '\0';
}

void stats_transmit_one(Stat_type_e stat)
{
    char line[22];

    switch (stat) {
        case STAT_DATE:
            date_update(&system_date);
            date_format(&system_date, line, sizeof(line));
            size_t len = strlen(line);
			line[len] = '\n';
			line[len + 1] = '\0';
            break;
        case STAT_DISTANCE:
        	update_distance();
            stat_line(line, "Distance:", "%04.1f cm", distance.filtered);
            break;
        case STAT_TEMPERATURE:
        	update_temperature();
            stat_line(line, "Temperature:", "%+05.1f C", temperature.filtered);
            break;
        case STAT_LIGHT:
        	light_update();
            stat_line(line, "Light:", "%4d lux", (int)light.lux);
            break;
        case STAT_X_ACCEL:
            stat_line(line, "X accel:", "%+5.2f g", 0.0f);
            break;
        case STAT_Y_ACCEL:
            stat_line(line, "Y accel:", "%+5.2f g", 0.0f);
            break;
        case STAT_Z_ACCEL:
            stat_line(line, "Z accel:", "%+5.2f g", 0.0f);
            break;
        case STAT_UNSAFE_DRIVING:
            stat_line(line, "Unsafe driving:", "%d", (int)0);
            break;
        case STAT_IMPACT_DETECTED:
            stat_line(line, "Impact detected:", "%d", (int)0);
            break;
        case STAT_LOW_LIGHT_WARNING:
        	light_update();
            stat_line(line, "Low-Light warning:", "%d", (int)light.warning);
            break;
        case STAT_PROXIMITY_WARNING:
        	update_distance();
            stat_line(line, "Proximity warning:", "%d", (int)distance.warning);
            break;
        case STAT_HIGH_TEMPERATURE:
        	update_temperature();
            stat_line(line, "High Temperature:", "%d", (int)temperature.warning);
            break;
        case STAT_GPS_LAT:
            stat_line(line, "GPSLat:", "%+09.6f", 0);
            break;
        case STAT_GPS_LONG:
            stat_line(line, "GPSLong:", "%+010.6f", 0);
            break;
        default:
            return;
    }

    UART_transmit(&g_uart2, (uint8_t *)line, strlen(line));
}

void stats_transmit_all()
{
	UART_transmit(&g_uart2, (uint8_t *)"@", 1);

    for (Stat_type_e s = STAT_DATE; s < STAT_COUNT; s++)
        stats_transmit_one(s);

    UART_transmit(&g_uart2, (uint8_t *)"&\n", 2);
}

