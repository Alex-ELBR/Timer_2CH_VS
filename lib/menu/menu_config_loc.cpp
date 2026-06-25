#include "menu_config_loc.h"



bool menu_config_loc(uint16_t button, eDisplay& displ, eDS1338& rtc)
{
    static uint8_t screen = 0;
    static uint8_t what_config = 0;

    switch(screen)
    {
        case SCREEN_CONF_LOC_LON:
        {
            displ.show("LOn ");
        }; break;

        case SCREEN_CONF_LOC_LAT:
        {
            displ.show("LAt ");
        }; break;

        case SCREEN_CONF_LOC_TMZ:
        {
            displ.show("UtC ");
        }; break;

        /*********************************************** */
        case ENTER_CONFIG_LOC:
        {
            switch(what_config)
            {
                case SCREEN_CONF_LOC_LON:
                {
                    if(!menu_config_loc_lon(button)) 
                    {
                        screen = SCREEN_CONF_LOC_LON;
                        return 1;
                    }
                    else return 1;

                }; break;

                case SCREEN_CONF_LOC_LAT:
                {
                    if(!menu_config_loc_lat(button)) 
                    {
                        screen = SCREEN_CONF_LOC_LAT;
                        return 1;
                    }
                    else return 1;

                }; break;

                case SCREEN_CONF_LOC_TMZ:
                {
                    
                    if(!menu_config_loc_timezone(button)) 
                    {
                        screen = SCREEN_CONF_LOC_TMZ;
                        return 1;
                    }
                    else return 1;
                    
                }; break;

                default:
                {
                    screen = SCREEN_CONF_LOC_LON;
                    return 1;
                }; break;
            }

        }; break;
    }


/******************************************************* */
    switch(button)
    {
        case eButton::PRESS_CANCEL:
        {
            return 0;
        }; break;

        case eButton::PRESS_UP:
        {
            if(screen < LAST_MENU_CONF_LOC - 1) ++screen;
            else screen = 0;
        }; break;

        case eButton::PRESS_DOWN:
        {
            if(screen > 0) --screen;
            else screen = (LAST_MENU_CONF_LOC - 1);
        }; break;

        case eButton::PRESS_OK:
        {
            what_config = screen;
            screen = ENTER_CONFIG_LOC;
        }; break;
    }   
    
    return 1;
}

