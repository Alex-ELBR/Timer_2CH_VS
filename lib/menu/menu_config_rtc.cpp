#include "menu_config_rtc.h"

extern Display displ;
extern eRTC rtc;


bool menu_config_rtc(uint16_t button)
{
    static uint8_t step_config;
    static uint8_t change_parameter;

    display_clock_t clocks;
    clocks.hour = rtc.get_hour();
    clocks.minute = rtc.get_minute();
    clocks.comma = true;



    switch(step_config)
    {
        case STEP_CONFIG_HOUR:
        {
            displ.show_clock(clocks, BLINK_HOUR);
            change_parameter = CHANGE_HOUR;
        }; break;

        case STEP_CONFIG_MIN:
        {
            displ.show_clock(clocks, BLINK_MIN);
            change_parameter = CHANGE_MINUTE;
        }; break;

        case STEP_CONFIG_YEAR:
        {
            displ.show_year(rtc.get_year(), BLINK);
            change_parameter = CHANGE_YEAR;
        }; break; 

        case STEP_CONFIG_MONTH:
        {
            displ.show_month(rtc.get_month(), BLINK);
            change_parameter = CHANGE_MONTH;
        }; break;

        case STEP_CONFIG_DATE:
        {
            displ.show_date(rtc.get_date(), BLINK);
            change_parameter = CHANGE_DATE;
        }; break;

    }


/******************************************************* */
    switch(button)
    {
        case PRESS_CANCEL:
        {
            rtc.stop_change();
            step_config = 0;
            return 0;
        }; break;

        case PRESS_UP:
        {
            rtc.change_parameter(change_parameter, PLUS);

        }; break;

        case PRESS_DOWN:
        {
            rtc.change_parameter(change_parameter, MINUS);

        }; break;

        case PRESS_OK:
        {
            if(step_config < STEP_APPLY_CONFIG_RTC - 1) ++step_config;
            else
            {
                rtc.change_parameter(0, APPLY_RTC);
                step_config = 0;
                return 0;
            }
        }; break;
    }   
    
    return 1;
}

