#include "menu_main.h"

extern Display displ;
extern eRTC rtc;



bool menu_main(uint16_t button)
{
    static uint8_t screen = SCREEN_CONF_RTC;
    static uint8_t what_config = 0;

    switch(screen)
    {
        case SCREEN_CONF_RTC:
        {
            displ.show("hour");
        }; break;

        case SCREEN_CONF_LOC:
        {
            displ.show("Loc ");
        }; break;

        case SCREEN_CONF_CH:
        {
            displ.show("CHnL");
        }; break;

        /*********************************************** */
        case ENTER_CONFIG:
        {
            switch(what_config)
            {
                case SCREEN_CONF_RTC:
                {
                    rtc.start_change();
                    if(!menu_config_rtc(button)) 
                    {
                        screen = SCREEN_CONF_RTC;
                        return 1;
                    }
                    else return 1;

                }; break;

                case SCREEN_CONF_LOC:
                {
                    if(!menu_config_loc(button)) 
                    {
                        screen = SCREEN_CONF_LOC;
                        return 1;
                    }
                    else return 1;

                }; break;

                case SCREEN_CONF_CH:
                {
                    if(!menu_config_chnl(button)) 
                    {
                        screen = SCREEN_CONF_CH;
                        return 1;
                    }
                    else return 1;

                }; break;

                default:
                {
                    screen = SCREEN_CONF_RTC;
                    return 1;
                }; break;
            }

        }; break;

    }
/******************************************************* */
    switch(button)
    {
        case PRESS_CANCEL:
        {
            screen = SCREEN_CONF_RTC;
            return 0;
        }; break;

        case PRESS_UP:
        {
            if(screen < LAST_MAIN_MENU - 1) ++screen;
            else screen = 0;
        }; break;

        case PRESS_DOWN:
        {
            if(screen > 0) --screen;
            else screen = (LAST_MAIN_MENU - 1);
        }; break;

        case PRESS_OK:
        {
            what_config = screen;
            screen = ENTER_CONFIG;
        }; break;
    }

    return 1;
}
