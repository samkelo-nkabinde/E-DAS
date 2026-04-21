/*
 * state_machine.c
 *
 * Created on: Apr 17, 2026
 * Author: Samkelo Nkabinde
 */

#include "state_machine.h"

state_e current_state = STATE_TOP_MENU;
top_menu_e current_top_menu = TOP_MEASUREMENTS;
measurements_page_e m_page = PAGE_1_1;
entry_page_e e_page = PAGE_2_1;
diagnostics_page_e d_page = PAGE_3_1;

int unsafe_driving = 0;
int impact_detected = 0;

void state_machine(void)
{
	date_update(&system_date);
	update_distance();
	light_update();
	update_temperature();

    if (current_state != STATE_WARNING)
    {
        if (unsafe_driving || impact_detected || light.warning || distance.warning || temperature.warning)
        {
            current_state = STATE_WARNING;
        }
    }

    ButtonType button = get_button_pressed();

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
                case PAGE_2_1: display_2_1(0); break;
                case PAGE_2_2: display_2_2(0); break;
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
            if (unsafe_driving) display_warn_unsafe_driving();
            else if (impact_detected) display_warn_impact();
            else if (light.warning) display_warn_low_light();
            else if (distance.warning) display_warn_proximity();
            else if (temperature.warning) display_warn_high_temp();

            if (button == BUTTON_CENTER)
            {
                unsafe_driving = 0;
                impact_detected = 0;
                light.warning = 0;
                distance.warning = 0;
                temperature.warning = 0;

                current_state = STATE_TOP_MENU;
            }
            break;
    }
}
