#include "menu_config_chnl.h"

extern Display displ;
extern eChannel channel[CHANNEL_AMOUNT];
extern eEEPROM eeprom;


bool menu_config_ch_func(uint16_t button, uint8_t ch_num)
{

    static uint8_t screen = channel[ch_num].get_channel_function();
    static bool disable_but_on_this_level = false;
    static channel_settings_t ch_settings = {0};


    switch(screen)
    {
        case SCREEN_CH_FUNC_OFF:
        {
            displ.show("0FF ");
            ch_settings.channel_func = CH_FUNC_OFF;
        }; break;

        case SCREEN_CH_FUNC_WEEK:
        {
            displ.show("UuEC");
            ch_settings.channel_func = CH_FUNC_WEEK;
        }; break;

        case SCREEN_CH_FUNC_SUN:
        {
            displ.show("Sun ");
            ch_settings.channel_func = CH_FUNC_SUN;
        }; break;

        case SCREEN_CH_FUNC_CIVIL:
        {
            displ.show("CuL ");
            ch_settings.channel_func = CH_FUNC_CIVIL;
        }; break;

        /*********************************************** */
        case ENTER_CONFIG_CH_FUNC:
        {
            channel[ch_num].set_channel_settings(&ch_settings);
            eeprom.write_channel_eeprom(channel[ch_num]);
            screen = channel[ch_num].get_channel_function();
            return 0;
        }; break;
    }

/******************************************************* */
    if(!disable_but_on_this_level)
    {
        switch(button)
        {
            case PRESS_CANCEL:
            {
                //screen = CH_FUNC_OFF;
                return 0;
            }; break;

            case PRESS_UP:
            {
                if(screen < LAST_MENU_CH_FUNC - 1) ++screen;
                else screen = 0;

            }; break;

            case PRESS_DOWN:
            {
                if(screen > 0) --screen;
                else screen = (LAST_MENU_CH_FUNC - 1);
            }; break;

            case PRESS_OK:
            {
                screen = ENTER_CONFIG_CH_FUNC;
                //disable_but_on_this_level = true;
            }; break;
        }     
    }
    return 1;



}
