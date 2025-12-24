#include "menu_config_loc.h"

extern Display displ;
extern eRTC rtc;


bool menu_config_loc_timezone(uint16_t button)
{
    static uint8_t step_config;
    static uint8_t change_parameter;


    switch(step_config)
    {
        case STEP_CONFIG_TIMEZONE:
        {
            displ.show_location_tz(rtc.get_timezone(), BLINK);
            change_parameter = CHANGE_TIMEZONE;
        }; break;

    }


/******************************************************* */
    switch(button)
    {
        case PRESS_CANCEL:
        {
            step_config = 0;
            change_parameter = 0;
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
            if(step_config < STEP_APPLY_CONFIG_TIMEZONE - 1) ++step_config;
            else
            {
                rtc.change_parameter(0, APPLY_LOC_TIMEZONE);
                step_config = 0;
                change_parameter = 0;
                return 0;
            }
        }; break;
    }   
    
    return 1;
}

