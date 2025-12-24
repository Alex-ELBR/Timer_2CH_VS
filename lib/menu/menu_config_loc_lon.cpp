#include "menu_config_loc.h"

extern Display displ;
extern eRTC rtc;


bool menu_config_loc_lon(uint16_t button)
{
    static uint8_t step_config;
    static uint8_t change_parameter;


    switch(step_config)
    {
        case STEP_CONFIG_LON_DEG:
        {
            displ.show_location_deg(rtc.get_longitude_degree(), BLINK);
            change_parameter = CHANGE_LON_DEG;
        }; break;

        case STEP_CONFIG_LON_MIN:
        {
            displ.show_location_min(rtc.get_longitude_min(), BLINK);
            change_parameter = CHANGE_LON_MIN;
        }; break;

        case STEP_CONFIG_LON_SEC:
        {
            displ.show_location_sec(rtc.get_longitude_sec(), BLINK);
            change_parameter = CHANGE_LON_SEC;
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
            if(step_config < STEP_APPLY_CONFIG_LOC_LON - 1) ++step_config;
            else
            {
                rtc.change_parameter(0, APPLY_LOC_LON);
                step_config = 0;
                change_parameter = 0;
                return 0;
            }
        }; break;
    }   
    
    return 1;
}

