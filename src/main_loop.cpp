#include <main_loop.h>

extern Display displ;
extern ELed led_1, led_2, led_3;
extern eRTC rtc;
extern EButton keyboard;
extern eEEPROM eeprom;
extern eChannel channel[CHANNEL_AMOUNT];
extern uint8_t test_month;

display_clock_t get_clock(eRTC rtc);
display_clock_t morning_twilight_start(eRTC rtc);
display_clock_t evening_twilight_end(eRTC rtc);


const uint8_t MAX_COUNT_MENU = 6;


enum STEP_STARTUP_LOAD
{
    STEP_LOAD_RTC_DATA = 0,
    STEP_LOAD_CH_DATA,
};

void main_loop(void)
{


    static uint8_t work_mode = STARTUP_LOAD;
    //static uint8_t status = MAIN_SCREEN;
    static uint8_t startup_load_step = 0;

    //static uint32_t ch_num = 0;

    switch(work_mode)
    {
    /************************************************/
        case STARTUP_LOAD:
        {
            switch(startup_load_step)
            {
                case STEP_LOAD_RTC_DATA:
                {
                    displ.show("LOAd");

                    led_1.on();
                    startup_load_step = STEP_LOAD_CH_DATA;

                }; break;

                case STEP_LOAD_CH_DATA:
                {
                    static uint8_t i = 0;
                    static uint8_t ch = 0;

                    if(ch < CHANNEL_AMOUNT)
                    {
                        if(i < TIMER_AMOUNT)
                        {
                            eeprom.read_timer_eeprom(channel[ch].timers[i]);
                            ++i;
                        }
                        else 
                        {
                            eeprom.read_channel_eeprom(channel[ch]);
                            ++ch;
                            i = 0;
                        }
                    }
                    else 
                    {
                        ch = 0;
                        i = 0;
                        work_mode = NORMAL_WORK;
                    }
                }; break;
               
            }

        }; break;
    /************************************************/
        case NORMAL_WORK:
        {
            static uint8_t screen = 0;
            switch (screen)
            {
                case 0:
                {
                    displ.show_clock(get_clock(rtc));
                };break;

                case 1:
                {
                    displ.show_date(rtc.get_date());
                };break;   

                case 2:
                {
                    displ.show_day(rtc.get_day());
                };break;

                case 3:
                {
                    displ.show_month(rtc.get_month());
                };break;  

                case 4:
                {
                    displ.show_year(rtc.get_year());
                };break;

                case 5:
                {
                    displ.show_clock(morning_twilight_start(rtc));
                };break;
                
                case 6:
                {
                    displ.show_clock(evening_twilight_end(rtc));
                };break;

                default:{ break; }
            }

            switch(keyboard.get_button())
            {
                case PRESS_UP:
                {
                    if(screen < MAX_COUNT_MENU) ++screen;
                    else screen = 0;
                };break;

                case PRESS_DOWN:
                {
                    if(screen > 0) --screen;
                    else screen = MAX_COUNT_MENU;
                };break;

                case PRESS_OK:
                {
                    work_mode = CONFIGURATION;
                };break;

                case PRESS_CANCEL:
                {

                };break;
            }                   
        }; break;

        case CONFIGURATION:
        {
            if(!menu_main(keyboard.get_button())) work_mode = NORMAL_WORK;           

        }; break;

            
    }


}

/************************************************************ */
display_clock_t get_clock(eRTC rtc)
{
    display_clock_t clocks;

    clocks.hour = rtc.get_hour();
    clocks.minute = rtc.get_minute();
    clocks.comma = rtc.get_sec_comma();
    return clocks;
}

display_clock_t morning_twilight_start(eRTC rtc)
{
    display_clock_t clocks;

    rtc.get_civil_dawn(clocks.hour, clocks.minute);
    clocks.comma = true;
    return clocks;
}

display_clock_t evening_twilight_end(eRTC rtc)
{
    display_clock_t clocks;
    
    rtc.get_civil_dusk(clocks.hour, clocks.minute);
    clocks.comma = true;

    return clocks;
}