#include "menu_config_chnl.h"

extern Display displ;
extern eRTC rtc;
extern eChannel channel[CHANNEL_AMOUNT];
extern eEEPROM eeprom;

display_clock_t convert_real_time_to_clock(timer_real_time_t real_time);


bool menu_config_ch_selected_tim(uint16_t button, uint8_t ch_num, uint8_t tim_num)
{
    static uint8_t step_config = STEP_CONFIG_TIM_ENABLE;
    static uint8_t change_parameter;
    timer_real_time_t real_time_on = {0};
    timer_real_time_t real_time_off = {0};
    bool enable = false;

    real_time_on = channel[ch_num].timers[tim_num].get_real_time_on();
    real_time_off = channel[ch_num].timers[tim_num].get_real_time_off();           
    enable = channel[ch_num].timers[tim_num].get_timer_enable();

    switch(step_config)
    {
        case STEP_CONFIG_TIM_ENABLE:
        {
            char sts;
            if(enable) sts = 'E';
            else sts = 'd';

            displ.show("5t-");
            displ.display_char_write_pos(sts, 1, BLINK);
            change_parameter = CHANGE_TIM_ENABLE;
        }; break;

        case STEP_CONFIG_HOUR_ON:
        {
            displ.show_clock(convert_real_time_to_clock(real_time_on), BLINK_HOUR);
            change_parameter = CHANGE_TIM_ON_HOUR;
        }; break;

        case STEP_CONFIG_MIN_ON:
        {
            displ.show_clock(convert_real_time_to_clock(real_time_on), BLINK_MIN);
            change_parameter = CHANGE_TIM_ON_MIN;
        }; break;

        case STEP_CONFIG_DAY_ON:
        {
            displ.show_day(real_time_on.day, BLINK);
            change_parameter = CHANGE_TIM_ON_DAY;
        }; break;

        case STEP_CONFIG_HOUR_OFF:
        {
            displ.show_clock(convert_real_time_to_clock(real_time_off), BLINK_HOUR);
            change_parameter = CHANGE_TIM_OFF_HOUR;
        }; break;

        case STEP_CONFIG_MIN_OFF:
        {
            //if(real_time_on.day == ALL_DAY) { step_config = STEP_APPLY_CONFIG_SELECTED_TIM; }

            displ.show_clock(convert_real_time_to_clock(real_time_off), BLINK_MIN);
            change_parameter = CHANGE_TIM_OFF_MIN;
        }; break;

        case STEP_CONFIG_DAY_OFF:
        {
            if(real_time_on.day == ALL_DAY) { step_config = STEP_APPLY_CONFIG_SELECTED_TIM; break; }
            
            if(real_time_off.day == ALL_DAY) { real_time_off.day = MONDAY; }
            displ.show_day(real_time_off.day, BLINK);
            change_parameter = CHANGE_TIM_OFF_DAY;
        }; break;

    }


/******************************************************* */
    switch(button)
    {
        case PRESS_CANCEL:
        {
            //rtc.stop_change();
            step_config = STEP_CONFIG_TIM_ENABLE;
            return 0;
        }; break;

        case PRESS_UP:
        {
            channel[ch_num].timers[tim_num].change_parameter(change_parameter, PLUS);

        }; break;

        case PRESS_DOWN:
        {
            channel[ch_num].timers[tim_num].change_parameter(change_parameter, MINUS);

        }; break;

        case PRESS_OK:
        {
            if(step_config < STEP_APPLY_CONFIG_SELECTED_TIM - 1) ++step_config;
            else
            {
                channel[ch_num].timers[tim_num].set_timer_settings();
                eeprom.write_timer_eeprom(channel[ch_num].timers[tim_num]);
                
                step_config = 0;
                return 0;
            }
        }; break;
    }   
    
    return 1;
}

//////////////////////////////////////////////////////

display_clock_t convert_real_time_to_clock(timer_real_time_t real_time)
{
    display_clock_t clock;
    clock.hour = real_time.hour;
    clock.minute = real_time.minute;
    clock.comma = true;
    return clock;
}