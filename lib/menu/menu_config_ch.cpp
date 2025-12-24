#include "menu_config_chnl.h"

extern Display displ;


bool menu_config_ch(uint16_t button, uint8_t ch_num)
{   
    static uint8_t screen = SCREEN_CONF_CH_FUNC;
    static uint8_t what_config = 0; 
    static bool disable_but_on_this_level = false;

    switch(screen)
    {
        case SCREEN_CONF_CH_FUNC:
        {
            displ.show("Func");
        }; break;

        case SCREEN_CONF_CH_TIM:
        {
            displ.show("CHtr");
        }; break;


        /*********************************************** */
        case ENTER_CONFIG_CH:
        {
            switch(what_config)
            {
                case SCREEN_CONF_CH_FUNC:
                {
                    /*
                    if(!menu_config_ch_func(button, ch_num))
                    {
                        screen = SCREEN_CONF_CH_FUNC;
                        disable_but_on_this_level = false;
                        return 1;
                    }
                    */
                }; break;

                case SCREEN_CONF_CH_TIM:
                {
                    
                    if(!menu_config_ch_tim(button, ch_num))
                    {
                        screen = SCREEN_CONF_CH_TIM;
                        disable_but_on_this_level = false;
                        return 1;
                    }
                        
                }; break;

                default:
                {
                    screen = SCREEN_CONF_CH_FUNC;
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
                screen = SCREEN_CONF_CH_FUNC;
                return 0;
            }; break;

            case PRESS_UP:
            {
                if(screen < LAST_MENU_CONF_CH - 1) ++screen;
                else screen = 0;
            }; break;

            case PRESS_DOWN:
            {
                if(screen > 0) --screen;
                else screen = (LAST_MENU_CONF_CH - 1);
            }; break;

            case PRESS_OK:
            {
                what_config = screen;
                screen = ENTER_CONFIG_CH;
                disable_but_on_this_level = true;
            }; break;
        }     
    }
    return 1;



}
