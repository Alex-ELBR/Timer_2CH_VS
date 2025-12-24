#include "menu_config_chnl.h"

extern Display displ;




bool menu_config_chnl(uint16_t button)
{
    static uint8_t screen = 0;
    static uint8_t ch_num = 1;
    static uint8_t what_config = 0;

    static bool disable_but_on_this_level = false;

    switch(screen)
    {
        case SCREEN_CONF_CH_NUM:
        {
            char i[] = {'C', 'H', '-', ch_num};
            displ.show(i);
        }; break;

        /*********************************************** */


        case ENTER_CONFIG_CHNL:
        {
            switch(what_config)
            {
                case SCREEN_CONF_CH_NUM:
                {
                    if(!menu_config_ch(button, ch_num))
                    {
                        screen = SCREEN_CONF_CH_NUM;
                        disable_but_on_this_level = false;
                        return 1;
                    }
                }; break;

                default:
                {
                    screen = SCREEN_CONF_CH_NUM;
                    return 1;
                }; break;
            }
        }; break;
    }

/******************************************************* */
    if(!disable_but_on_this_level)
    {
        switch(button)
        {
            case PRESS_CANCEL:
            {
                ch_num = 1;
                screen = SCREEN_CONF_CH_NUM;
                return 0;
            }; break;

            case PRESS_UP:
            {
                if(ch_num < CHANNEL_AMOUNT) 
                {
                    ++ch_num;
                    screen = SCREEN_CONF_CH_NUM;
                }
                else 
                {
                    ch_num = 1;
                    screen = SCREEN_CONF_CH_NUM;
                }
            }; break;

            case PRESS_DOWN:
            {
                if(ch_num > CHANNEL_AMOUNT) 
                {
                    --ch_num;
                    screen = SCREEN_CONF_CH_NUM;
                }
                else 
                {
                    ch_num = CHANNEL_AMOUNT;
                    screen = SCREEN_CONF_CH_NUM;
                }
            }; break;

            case PRESS_OK:
            {
                
                //rtc.start_change();
                what_config = screen;
                screen = ENTER_CONFIG_CHNL;
                disable_but_on_this_level = true;
                
            }; break;
        }     
    }
    return 1;

}
