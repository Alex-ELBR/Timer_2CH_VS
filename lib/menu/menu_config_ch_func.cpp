#include "menu_config_chnl.h"

extern Display displ;
extern eChannel channel[CHANNEL_AMOUNT];
extern eEEPROM eeprom;


bool menu_config_ch_func(uint16_t button, uint8_t ch_num)
{

    static uint8_t ch_func = 0;
    static bool disable_but_on_this_level = false;
    //static channel_settings_t ch_settings = {0};


    ch_func = channel[ch_num].get_channel_function();
    if(ch_func > CH_FUNC_CIVIL) ch_func = CH_FUNC_OFF;

    switch(ch_func)
    {
        case CH_FUNC_OFF:
        {
            displ.show("0FF ");
        }; break;

        case CH_FUNC_WEEK:
        {
            displ.show("UuEC");
        }; break;

        case CH_FUNC_SUN:
        {
            displ.show("Sun ");
        }; break;

        case CH_FUNC_CIVIL:
        {
            displ.show("CuL ");
        }; break;

        /*********************************************** 
        case ENTER_CONFIG_CH_FUNC:
        {
            eeprom.write_channel_eeprom(channel[ch_num]);
            screen = channel[ch_num].get_channel_function();
            return 0;
        }; break;
        */
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
                channel[ch_num].change_parameter(CHANGE_CH_FUNCTION, PLUS);
            }; break;

            case PRESS_DOWN:
            {
                channel[ch_num].change_parameter(CHANGE_CH_FUNCTION, MINUS);
            }; break;

            case PRESS_OK:
            {
                channel[ch_num].set_channel_settings();
                eeprom.write_channel_eeprom(channel[ch_num]);
                return 0;
            }; break;
        }     
    }
    return 1;



}
