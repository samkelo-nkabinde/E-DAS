/*
 * state_machine.c
 *
 * Created on: Apr 17, 2026
 * Author: Samkelo Nkabinde
 */

#include "state_machine.h"

static state_e current_state = STATE_TOP_MENU;
static top_menu_e current_top_menu = TOP_MEASUREMENTS;
static measurements_page_e m_page = PAGE_1_1;
static entry_page_e e_page = PAGE_2_1;
static diagnostics_page_e d_page = PAGE_3_1;

static state_e prev_state;
static top_menu_e prev_top_menu;
static measurements_page_e prev_m_page;
static entry_page_e prev_e_page;
static diagnostics_page_e prev_d_page;

int unsafe_driving = 0;
int impact_detected = 0;

static uint8_t locked = 0;
static ButtonType last_button = NO_BUTTON;

static keypad_num_t kp_2_1 = {0, 0, 0};
static keypad_num_t kp_2_2 = {0, 0, 0};

void state_machine(void)
{
	date_update(&system_date);
	update_distance();
	light_update();
	update_temperature();
	compute_fuel_efficiency();

	static uint8_t light_counter = 0;
	light_counter++;
	if (light_counter >= 6)
	{
		light_update();
		light_counter = 0;
	}

	if (current_state == STATE_WARNING)
	{
	    if (distance.filtered > 30)
	    {
	        distance.warning = 0;

	        if (!unsafe_driving && !impact_detected && !light.warning && !temperature.warning)
	        {
	        	distance_external_warning = 0;

	            current_state = prev_state;
	            current_top_menu = prev_top_menu;
	            m_page = prev_m_page;
	            e_page = prev_e_page;
	            d_page = prev_d_page;

	            last_button = NO_BUTTON;
	            LED_off(&D2);
	        }
	    }
	}

	if (current_state != STATE_WARNING)
	{
	    if (unsafe_driving || impact_detected || light.warning || distance.warning || temperature.warning)
	    {
	        prev_state = current_state;
	        prev_top_menu = current_top_menu;
	        prev_m_page = m_page;
	        prev_e_page = e_page;
	        prev_d_page = d_page;

	        current_state = STATE_WARNING;
	    }
	}

    ButtonType button = get_button_pressed();

    if (current_state == STATE_ENTRY)
    {
        if (locked)
        {
            if (e_page == PAGE_2_1) keypad_number_update(&kp_2_1);
            else if (e_page == PAGE_2_2) keypad_number_update(&kp_2_2);
        }

        if (button == BUTTON_CENTER && last_button != BUTTON_CENTER)
        {
            if (locked)
            {
                if (e_page == PAGE_2_1)
                {
                    keypad_number_commit(&kp_2_1);
                    last_liters = (float)kp_2_1.last;
                }
                else if (e_page == PAGE_2_2)
                {
                    keypad_number_commit(&kp_2_2);
                    last_distance = (float)kp_2_2.last;
                }
            }
            locked = !locked;
        }
        if (locked && button != BUTTON_CENTER)
        {
            button = NO_BUTTON;
        }
    }
    else
    {
        locked = 0;
    }

    switch (current_state)
    {
        case STATE_TOP_MENU:
            if (current_top_menu == TOP_MEASUREMENTS) display_default_page();
            else if (current_top_menu == TOP_ENTRY) display_entry_page();
            else if (current_top_menu == TOP_DIAGNOSTICS) display_diagnostics_page();

            if (button != NO_BUTTON)
            {
                if (button == BUTTON_UP)
                {
                    current_top_menu = (current_top_menu == TOP_MEASUREMENTS) ? TOP_DIAGNOSTICS : current_top_menu - 1;
                }
                else if (button == BUTTON_DOWN)
                {
                    current_top_menu = (current_top_menu + 1) % 3;
                }
                else if (button == BUTTON_RIGHT)
                {
                    if (current_top_menu == TOP_MEASUREMENTS) current_state = STATE_MEASUREMENTS;
                    else if (current_top_menu == TOP_ENTRY) current_state = STATE_ENTRY;
                    else if (current_top_menu == TOP_DIAGNOSTICS) current_state = STATE_DIAGNOSTICS;
                }
            }
            break;

        case STATE_MEASUREMENTS:
            switch (m_page)
            {
                case PAGE_1_1: display_1_1(); break;
                case PAGE_1_2: display_1_2(); break;
                case PAGE_1_3: display_1_3(); break;
                case PAGE_1_4: display_1_4(); break;
                case PAGE_1_5: display_1_5(); break;
            }
            if (button != NO_BUTTON)
            {
                if (button == BUTTON_UP) m_page = (m_page == PAGE_1_1) ? PAGE_1_5 : m_page - 1;
                else if (button == BUTTON_DOWN) m_page = (m_page + 1) % 5;
                else if (button == BUTTON_LEFT) current_state = STATE_TOP_MENU;
            }
            break;

        case STATE_ENTRY:
            switch (e_page)
            {
            	case PAGE_2_1: display_2_1(locked, kp_2_1.current); break;
            	case PAGE_2_2: display_2_2(locked, kp_2_2.current); break;
                case PAGE_2_3: display_2_3(0); break;
            }
            if (button != NO_BUTTON)
            {
            	if (button == BUTTON_UP)
            	{
            	    if (e_page == PAGE_2_1) e_page = PAGE_2_3;
            	    else e_page--;
            	}
            	else if (button == BUTTON_DOWN)
                {
                    if (e_page == PAGE_2_3) e_page = PAGE_2_1;
                    else e_page++;
                }
                else if (button == BUTTON_LEFT) current_state = STATE_TOP_MENU;
            }
            break;

        case STATE_DIAGNOSTICS:
            switch (d_page)
            {
                case PAGE_3_1: display_3_1(1); break;
                case PAGE_3_2: display_3_2(1); break;
                case PAGE_3_3: display_3_3(1); break;
                case PAGE_3_4: display_3_4(1); break;
            }
            if (button != NO_BUTTON)
            {
                if (button == BUTTON_UP) d_page = (d_page == PAGE_3_1) ? PAGE_3_4 : d_page - 1;
                else if (button == BUTTON_DOWN) d_page = (d_page + 1) % 4;
                else if (button == BUTTON_LEFT) current_state = STATE_TOP_MENU;
            }
            break;

        case STATE_WARNING:
            if (unsafe_driving)
            {
            	display_warn_unsafe_driving();
            }
            else if (impact_detected)
            {
            	display_warn_impact();
            }
            else if (light.warning)
            {
            	LED_blink_control(&D4);
            	display_warn_low_light();
            }
            else if (distance.warning)
            {
            	LED_blink_control(&D2);
            	display_warn_proximity();
            }
            else if (temperature.warning)
            {
            	LED_blink_control(&D5);
            	display_warn_high_temp();
            }

            if (button == BUTTON_CENTER && last_button != BUTTON_CENTER)
            {
            	distance_external_warning = 0;
            	light_external_warning = 0;
            	temperature_external_warning = 0;

                unsafe_driving = 0;
                impact_detected = 0;
                light.warning = 0;
                distance.warning = 0;
                temperature.warning = 0;

                current_state = prev_state;

                current_top_menu = prev_top_menu;
                m_page = prev_m_page;
                e_page = prev_e_page;
                d_page = prev_d_page;

                LED_off(&D5);
                LED_off(&D4);
                button = NO_BUTTON;
            }
            break;
    }

    last_button = button;
}
