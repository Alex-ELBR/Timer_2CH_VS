#include <main_loop.h>

extern Display displ;
extern ELed led_1, led_2, led_3;
extern eRTC rtc;
extern EButton keyboard;
extern eEEPROM eeprom;
extern eChannel channel[CHANNEL_AMOUNT];
extern uint8_t test_month;

display_clock_t get_clock(eRTC rtc);

const uint8_t MAX_COUNT_MENU = 4;


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
                    uint8_t i = 0;

                    //if(eeprom.read_timer_eeprom(channel[0].timers[0]) == HAL_OK) {work_mode = NORMAL_WORK;}

                    //work_mode = NORMAL_WORK;
                    
                    while(i < TIMER_AMOUNT)
                    {
                        eeprom.read_timer_eeprom(channel[0].timers[i]);
                        ++i;
                    }
                    
                    work_mode = NORMAL_WORK;

                    
                    

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
                    //displ.show((uint32_t)(channel[0].timers[0].get_timer_settings_address()));
                    displ.show_date(rtc.get_date());
                };break;   

                case 2:
                {
                    
                    //displ.show((uint32_t)(channel[0].timers[1].get_timer_settings_address()));
                    displ.show_day(rtc.get_day());
                };break;

                case 3:
                {
                    
                    //displ.show((uint32_t)(channel[0].timers[TIMER_AMOUNT - 1].get_timer_settings_address()));

                    displ.show_month(rtc.get_month());
                };break;  

                case 4:
                {
                    //displ.show((uint32_t)(channel[CHANNEL_AMOUNT - 1].timers[0].get_timer_settings_address()));

                    displ.show_year(rtc.get_year());
                };break;

                case 5:
                {
                    displ.show((uint32_t)(channel[CHANNEL_AMOUNT - 1].timers[1].get_timer_settings_address()));
                };break;
                
                case 6:
                {
                    displ.show((uint32_t)(channel[CHANNEL_AMOUNT - 1].timers[TIMER_AMOUNT - 1].get_timer_settings_address()));
                };break;

                case 7:
                {
                    displ.show((uint32_t)(channel[0].get_channel_settings_address()));
                };break;

                case 8:
                {
                    displ.show((uint32_t)(channel[CHANNEL_AMOUNT - 1].get_channel_settings_address()));
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