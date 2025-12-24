#include "menu_config_chnl.h"

extern Display displ;


bool menu_config_ch_tim(uint16_t button, uint8_t ch_num)
{   
    static uint8_t screen = SCREEN_CONF_TIM;
    static uint8_t tim_num = 1;
    static uint8_t what_config = 0; 
    static bool disable_but_on_this_level = false;

    bcd8_level_t tim_num_digit;


    switch(screen)
    {
        case SCREEN_CONF_TIM:
        {
            tim_num_digit = bin8_trans(tim_num);

            char i[] = {'t', char(ch_num), '.', char(tim_num_digit.tens), char(tim_num_digit.units)};
            displ.show(i);
        }; break;


        /*********************************************** */
        case ENTER_CONFIG_TIM:
        {
            switch(what_config)
            {
                case SCREEN_CONF_TIM:
                {
                
                    if(!menu_config_ch_selected_tim(button, ch_num - 1, tim_num - 1))
                    {
                        disable_but_on_this_level = false;
                        screen = SCREEN_CONF_TIM;
                        return 1;                        
                    }
                
/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
                    disable_but_on_this_level = false;
                    screen = SCREEN_CONF_TIM;
                    return 0;
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

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
                tim_num = 1;
                screen = SCREEN_CONF_TIM;
                return 0;
            }; break;

            case PRESS_UP:
            {
                if(tim_num < TIMER_AMOUNT) 
                {
                    ++tim_num;
                    screen = SCREEN_CONF_TIM;
                }
                else 
                {
                    tim_num = 1;
                    screen = SCREEN_CONF_TIM;
                }

            }; break;

            case PRESS_DOWN:
            {
                if(tim_num > 1) --tim_num;
                else 
                {
                    tim_num = TIMER_AMOUNT;
                    screen = SCREEN_CONF_TIM;
                }
            }; break;

            case PRESS_OK:
            {
                what_config = screen;
                screen = ENTER_CONFIG_TIM;
                disable_but_on_this_level = true;
            }; break;
        }     
    }
    return 1;



}
