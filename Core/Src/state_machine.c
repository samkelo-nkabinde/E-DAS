///*
// * state_machine.c
// *
// *  Created on: Apr 17, 2026
// *      Author: Samkelo Nkabinde
// */
//
//#include "state_machine.h"
//
//Page_t   page_list[PAGE_COUNT];
//uint8_t  page_index    = DEFAULT;
//uint8_t  subpage_index = 0;
//NavLevel nav_level     = LEVEL_TOP;
//uint8_t  active_warning = WARN_NONE;
//
//SubPage_t measurements_subpages[DEFAULT_SUBPAGE_COUNT];
//SubPage_t data_entry_subpages[DATA_ENTRY_SUBPAGE_COUNT];
//SubPage_t diagnostics_subpages[DIAGNOSTICS_SUBPAGE_COUNT];
//
//static uint32_t low_light_cooldown_start = 0;
//static uint32_t high_temp_cooldown_start = 0;
//static bool     low_light_in_cooldown    = false;
//static bool     high_temp_in_cooldown    = false;
//
//#define LOW_LIGHT_COOLDOWN_MS   (60UL * 60UL * 1000UL)
//#define HIGH_TEMP_COOLDOWN_MS   (30UL * 60UL * 1000UL)
//
//static char  entry_buffer[MAX_OLED_CHAR];
//static uint8_t entry_len = 0;
//
//static void refresh_datetime(char *buf)
//{
//
//    get_datetime_string(buf);
//}
//
//static uint8_t wrap(int16_t index, uint8_t max)
//{
//    if (index < 0)  return max - 1;
//    if (index >= max) return 0;
//    return (uint8_t)index;
//}
//
//void set_page(Page_t *page, const char *title,
//              const char *instruction, PageState state,
//              SubPage_t *subpages, uint8_t subpage_count)
//{
//    strncpy(page->title, title, MAX_OLED_CHAR - 1);
//    strncpy(page->instruction, instruction, MAX_OLED_CHAR - 1);
//    page->state = state;
//    page->subpage_list = subpages;
//    page->subpage_count = subpage_count;
//}
//
//void display_page(Page_t *page)
//{
//    char dt[MAX_OLED_CHAR];
//    refresh_datetime(dt);
//    OLED_clear();
//    OLED_write(dt, 0, 0);
//    OLED_write(page->title,      0, 12);
//    OLED_write(page->instruction,0, 22);
//}
//
//void display_subpage(SubPage_t *subpage)
//{
//    OLED_clear();
//    OLED_write(subpage->date, 0, 0);
//    OLED_write(subpage->quantity_1, 0, 12);
//    OLED_write(subpage->quantity_2, 0, 22);
//}
//
//void SM_init(void)
//{
//
//    set_page(&page_list[DEFAULT],
//             "== Measurements ==", "Press -> display",
//             DISPLAYED,
//             measurements_subpages, DEFAULT_SUBPAGE_COUNT);
//
//    set_page(&page_list[DATA_ENTRY],
//             "=== Data Entry ===", "Press -> display",
//             NOT_DISPLAYED,
//             data_entry_subpages, DATA_ENTRY_SUBPAGE_COUNT);
//
//    set_page(&page_list[DIAGNOSTICS],
//             "== Diagnostics ===", "Press -> display",
//             NOT_DISPLAYED,
//             diagnostics_subpages, DIAGNOSTICS_SUBPAGE_COUNT);
//
//
//    strncpy(measurements_subpages[0].quantity_1, "Dist: -- cm",    MAX_OLED_CHAR-1);
//    strncpy(measurements_subpages[0].quantity_2, "Temp: -- C",     MAX_OLED_CHAR-1);
//
//    strncpy(measurements_subpages[1].quantity_1, "Accel: |-.--| g",MAX_OLED_CHAR-1);
//    strncpy(measurements_subpages[1].quantity_2, "Light: ---- lux",MAX_OLED_CHAR-1);
//
//    strncpy(measurements_subpages[2].quantity_1, "Lat: --.------", MAX_OLED_CHAR-1);
//    strncpy(measurements_subpages[2].quantity_2, "Long: ---.------",MAX_OLED_CHAR-1);
//
//    strncpy(measurements_subpages[3].quantity_1, "Heading: --- deg",MAX_OLED_CHAR-1);
//    strncpy(measurements_subpages[3].quantity_2, "Speed: ---.- km/h",MAX_OLED_CHAR-1);
//
//
//    strncpy(measurements_subpages[4].date,       "Fuel Efficiency:", MAX_OLED_CHAR-1);
//    strncpy(measurements_subpages[4].quantity_1, "--.-- km/L",      MAX_OLED_CHAR-1);
//    strncpy(measurements_subpages[4].quantity_2, "--.-- L/100km",   MAX_OLED_CHAR-1);
//
//    strncpy(data_entry_subpages[0].date,       "Enter fuel liters", MAX_OLED_CHAR-1);
//    strncpy(data_entry_subpages[0].quantity_1, "Current: ---.-- L", MAX_OLED_CHAR-1);
//    strncpy(data_entry_subpages[0].quantity_2, "Press S3 to change",MAX_OLED_CHAR-1);
//    data_entry_subpages[0].state = NOT_EDITING;
//
//    strncpy(data_entry_subpages[1].date,       "Enter odometer km", MAX_OLED_CHAR-1);
//    strncpy(data_entry_subpages[1].quantity_1, "Current: ---.- km", MAX_OLED_CHAR-1);
//    strncpy(data_entry_subpages[1].quantity_2, "Press S3 to change",MAX_OLED_CHAR-1);
//    data_entry_subpages[1].state = NOT_EDITING;
//
//    strncpy(data_entry_subpages[2].date,       "Log data (Y/N)",    MAX_OLED_CHAR-1);
//    strncpy(data_entry_subpages[2].quantity_1, "'*' = Y / '#' = N", MAX_OLED_CHAR-1);
//    strncpy(data_entry_subpages[2].quantity_2, "Log Data: DISABLED",MAX_OLED_CHAR-1);
//    data_entry_subpages[2].state = NOT_EDITING;
//
//    strncpy(diagnostics_subpages[0].quantity_1, "-- Diagnostics ---",MAX_OLED_CHAR-1);
//    strncpy(diagnostics_subpages[0].quantity_2, "SD-card: NOT OK",   MAX_OLED_CHAR-1);
//
//    strncpy(diagnostics_subpages[1].quantity_1, "-- Diagnostics ---",MAX_OLED_CHAR-1);
//    strncpy(diagnostics_subpages[1].quantity_2, "MPU-6050: NOT OK",  MAX_OLED_CHAR-1);
//
//    strncpy(diagnostics_subpages[2].quantity_1, "-- Diagnostics ---",MAX_OLED_CHAR-1);
//    strncpy(diagnostics_subpages[2].quantity_2, "GPS: NOT OK",       MAX_OLED_CHAR-1);
//
//    strncpy(diagnostics_subpages[3].quantity_1, "-- Diagnostics ---",MAX_OLED_CHAR-1);
//    strncpy(diagnostics_subpages[3].quantity_2, "Log Data: DISABLED",MAX_OLED_CHAR-1);
//
//
//    display_page(&page_list[DEFAULT]);
//}
//
//static void refresh_sensor_subpages(void)
//{
//    char dt[MAX_OLED_CHAR];
//    refresh_datetime(dt);
//
//
//    strncpy(measurements_subpages[0].date, dt, MAX_OLED_CHAR-1);
//    snprintf(measurements_subpages[0].quantity_1, MAX_OLED_CHAR,
//             "Dist: %.1f cm",  get_distance_cm());
//    snprintf(measurements_subpages[0].quantity_2, MAX_OLED_CHAR,
//             "Temp: %.1f C",   get_temperature_c());
//
//    strncpy(measurements_subpages[1].date, dt, MAX_OLED_CHAR-1);
//    snprintf(measurements_subpages[1].quantity_1, MAX_OLED_CHAR,
//             "Accel: |%.2f| g",get_accel_g());
//    snprintf(measurements_subpages[1].quantity_2, MAX_OLED_CHAR,
//             "Light: %4d lux", (int)get_light_lux());
//
//    strncpy(measurements_subpages[2].date, dt, MAX_OLED_CHAR-1);
//    snprintf(measurements_subpages[2].quantity_1, MAX_OLED_CHAR,
//             "Lat: %.6f",      get_latitude());
//    snprintf(measurements_subpages[2].quantity_2, MAX_OLED_CHAR,
//             "Long: %.6f",     get_longitude());
//
//
//    strncpy(measurements_subpages[3].date, dt, MAX_OLED_CHAR-1);
//    snprintf(measurements_subpages[3].quantity_1, MAX_OLED_CHAR,
//             "Heading: %3d deg",(int)get_heading_deg());
//    snprintf(measurements_subpages[3].quantity_2, MAX_OLED_CHAR,
//             "Speed: %.1f km/h",get_speed_kmh());
//
//
//    snprintf(measurements_subpages[4].quantity_1, MAX_OLED_CHAR,
//             "%.1f km/L",      get_fuel_efficiency_kml());
//    snprintf(measurements_subpages[4].quantity_2, MAX_OLED_CHAR,
//             "%.1f L/100km",   get_fuel_efficiency_l100());
//
//
//    char dt2[MAX_OLED_CHAR];
//    refresh_datetime(dt2);
//    for (int i = 0; i < DIAGNOSTICS_SUBPAGE_COUNT; i++)
//        strncpy(diagnostics_subpages[i].date, dt2, MAX_OLED_CHAR-1);
//
//    snprintf(diagnostics_subpages[0].quantity_2, MAX_OLED_CHAR,
//             "SD-card: %s",  sd_card_ok()   ? "OK" : "NOT OK");
//    snprintf(diagnostics_subpages[1].quantity_2, MAX_OLED_CHAR,
//             "MPU-6050: %s", mpu6050_ok()   ? "OK" : "NOT OK");
//    snprintf(diagnostics_subpages[2].quantity_2, MAX_OLED_CHAR,
//             "GPS: %s",      gps_ok()       ? "OK" : "NOT OK");
//    snprintf(diagnostics_subpages[3].quantity_2, MAX_OLED_CHAR,
//             "Log Data: %s", data_logging_enabled() ? "ENABLED" : "DISABLED");
//}
//
//static void data_entry_digit(char digit, uint8_t sub_index)
//{
//    if (entry_len < 5)
//    {
//        entry_buffer[entry_len++] = digit;
//        entry_buffer[entry_len]   = '\0';
//    }
//
//    if (sub_index == 0)
//        snprintf(data_entry_subpages[0].quantity_1, MAX_OLED_CHAR,
//                 "Current: %s L", entry_buffer);
//    else
//        snprintf(data_entry_subpages[1].quantity_1, MAX_OLED_CHAR,
//                 "Current: %s km", entry_buffer);
//
//    display_subpage(&data_entry_subpages[sub_index]);
//}
//
//static void data_entry_accept(uint8_t sub_index)
//{
//    float val = atof(entry_buffer);
//    entry_len = 0;
//    memset(entry_buffer, 0, sizeof(entry_buffer));
//
//    if (sub_index == 0)
//        set_fuel_liters(val);
//    else
//        set_odometer_km(val);
//
//    recalculate_fuel_efficiency();
//
//    data_entry_subpages[sub_idx].state = NOT_EDITING;
//    strncpy(data_entry_subpages[sub_index].quantity_2,
//            "Press S3 to change", MAX_OLED_CHAR-1);
//    display_subpage(&data_entry_subpages[sub_index]);
//}
//
//void SM_display_warning(WarningFlag warning)
//{
//    OLED_clear();
//    switch (warning)
//    {
//        case WARN_UNSAFE:
//            OLED_write("!! WARNING !!",   0, 0);
//            OLED_write("Unsafe Driving",  0, 12);
//            OLED_write("Press S3 to clear",0, 22);
//            break;
//        case WARN_LOW_LIGHT:
//            OLED_write("!! WARNING !!",   0, 0);
//            OLED_write("Low Light",       0, 12);
//            OLED_write("Press S3 to clear",0, 22);
//            break;
//        case WARN_PROX:
//            OLED_write("!! WARNING !!",   0, 0);
//            OLED_write("Proximity Dist.", 0, 12);
//            OLED_write("Press S3 to clear",0, 22);
//            break;
//        case WARN_HIGH_TEMP:
//            OLED_write("!! WARNING !!",   0, 0);
//            OLED_write("High Temp!",      0, 12);
//            OLED_write("Press S3 to clear",0, 22);
//            break;
//        default: break;
//    }
//}
//
//void SM_check_warnings(void)
//{
//    uint32_t now = HAL_GetTick();
//
//    if (get_accel_g() > 0.5f)
//        active_warning |= WARN_UNSAFE;
//
//    if (!low_light_in_cooldown && get_light_lux() <= 300.0f)
//        active_warning |= WARN_LOW_LIGHT;
//    if (low_light_in_cooldown &&
//        (now - low_light_cooldown_start >= LOW_LIGHT_COOLDOWN_MS))
//        low_light_in_cooldown = false;
//
//    if (get_distance_cm() <= 10.0f)
//        active_warning |= WARN_PROX;
//    else if (get_distance_cm() > 30.0f)
//        active_warning &= ~WARN_PROX;
//
//    if (!high_temp_in_cooldown && get_temperature_c() > 30.0f)
//        active_warning |= WARN_HIGH_TEMP;
//    if (active_warning & WARN_HIGH_TEMP && get_temperature_c() < 30.0f)
//    {
//        active_warning      &= ~WARN_HIGH_TEMP;
//        high_temp_in_cooldown = true;
//        high_temp_cooldown_start = now;
//    }
//    if (high_temp_in_cooldown &&
//        (now - high_temp_cooldown_start >= HIGH_TEMP_COOLDOWN_MS))
//        high_temp_in_cooldown = false;
//}
//
//void SM_tick(void)
//{
//    refresh_sensor_subpages();
//    SM_check_warnings();
//
//
//    if (active_warning != WARN_NONE)
//    {
//
//        WarningFlag shown = WARN_NONE;
//        if (active_warning & WARN_PROX)     shown = WARN_PROX;
//        else if (active_warning & WARN_UNSAFE)   shown = WARN_UNSAFE;
//        else if (active_warning & WARN_HIGH_TEMP)shown = WARN_HIGH_TEMP;
//        else if (active_warning & WARN_LOW_LIGHT)shown = WARN_LOW_LIGHT;
//
//        SM_display_warning(shown);
//
//
//        ButtonType btn = get_button_pressed();
//        if (btn == BUTTON_S3)
//        {
//            uint32_t now = HAL_GetTick();
//
//            if (shown == WARN_LOW_LIGHT)
//            {
//                active_warning       &= ~WARN_LOW_LIGHT;
//                low_light_in_cooldown = true;
//                low_light_cooldown_start = now;
//            }
//            else if (shown == WARN_HIGH_TEMP)
//            {
//                active_warning        &= ~WARN_HIGH_TEMP;
//                high_temp_in_cooldown  = true;
//                high_temp_cooldown_start = now;
//            }
//            else
//            {
//                active_warning &= ~shown;
//            }
//        }
//        return;
//    }
//
//
//    Page_t *cur_page    = &page_list[page_index];
//    SubPage_t *cur_subpage = &cur_page->subpage_list[subpage_index];
//    ButtonType button      = get_button_pressed();
//
//    switch (nav_level)
//    {
//
//        case LEVEL_TOP:
//        {
//            switch (button)
//            {
//                case BUTTON_S1_UP:
//                    page_index = wrap((int16_t)page_index - 1, PAGE_COUNT);
//                    display_page(&page_list[page_index]);
//                    break;
//
//                case BUTTON_S5_DOWN:
//                    page_index = wrap((int16_t)page_index + 1, PAGE_COUNT);
//                    display_page(&page_list[page_index]);
//                    break;
//
//                case BUTTON_S4_RIGHT:
//                    nav_level     = LEVEL_SUB;
//                    subpage_index = 0;
//                    display_subpage(&page_list[page_index].subpage_list[0]);
//                    break;
//
//                default: break;
//            }
//            break;
//        }
//
//        case LEVEL_SUB:
//        {
//            uint8_t sub_count = cur_page->subpage_count;
//
//            if (page_index == DATA_ENTRY &&
//                subpage_index < 2 &&
//                cur_subpage->state == EDITING)
//            {
//                char digit = get_keypad_digit();
//                if (digit >= '0' && digit <= '9')
//                {
//                    data_entry_digit(digit, subpage_index);
//                    return;
//                }
//            }
//
//            switch (button)
//            {
//                case BUTTON_S1_UP:
//                    subpage_index = wrap((int16_t)subpage_index - 1, sub_count);
//                    display_subpage(&cur_page->subpage_list[subpage_index]);
//                    break;
//
//                case BUTTON_S5_DOWN:
//                    subpage_index = wrap((int16_t)subpage_index + 1, sub_count);
//                    display_subpage(&cur_page->subpage_list[subpage_index]);
//                    break;
//
//                case BUTTON_S2_LEFT:
//                    nav_level = LEVEL_TOP;
//                    display_page(cur_page);
//                    break;
//
//                case BUTTON_S3:
//                    if (page_index == DATA_ENTRY && subpage_index < 2)
//                    {
//                        if (cur_subpage->state == NOT_EDITING)
//                        {
//                            cur_subpage->state = EDITING;
//                            entry_len = 0;
//                            memset(entry_buffer, 0, sizeof(entry_buffer));
//                            strncpy(cur_subpage->quantity_2,
//                                    "Press S3 to accept", MAX_OLED_CHAR-1);
//                            display_subpage(cur_subpage);
//                        }
//                        else
//                        {
//                            data_entry_accept(subpage_index);
//                        }
//                    }
//                    break;
//
//                case BUTTON_KEYPAD_STAR:
//                    if (page_index == DATA_ENTRY && subpage_index == 2)
//                    {
//                        enable_data_logging();
//                        strncpy(data_entry_subpages[2].quantity_2,
//                                "Log Data: ENABLED", MAX_OLED_CHAR-1);
//                        display_subpage(&data_entry_subpages[2]);
//                    }
//                    break;
//
//                case BUTTON_KEYPAD_HASH:
//                    if (page_index == DATA_ENTRY && subpage_index == 2)
//                    {
//                        disable_data_logging();
//                        strncpy(data_entry_subpages[2].quantity_2,
//                                "Log Data: DISABLED", MAX_OLED_CHAR-1);
//                        display_subpage(&data_entry_subpages[2]);
//                    }
//                    break;
//
//                default: break;
//            }
//            break;
//        }
//    }
//}
