#include <main_loop.h>

extern eDisplay displ;
extern ELed led_1, led_2, led_3;
extern eDS1338 rtc;
extern eButton keyboard;
extern eEEPROM eeprom;
extern eChannel channel[CHANNEL_AMOUNT];
extern eOLED oled;
extern uint8_t test_month;

display_clock_t get_clock(eDS1338& rtc);
display_clock_t morning_twilight_start(eDS1338& rtc);
display_clock_t evening_twilight_end(eDS1338& rtc);


const uint8_t MAX_COUNT_MENU = 6;

eMenu::Context ctx = { displ, rtc, oled }; 




enum STEP_STARTUP_LOAD
{
    STEP_LOAD_RTC_DATA = 0,
    STEP_LOAD_CH_DATA,
};

void main_loop(void)
{
    static uint8_t work_mode = STARTUP_LOAD;
    static uint8_t startup_load_step = 0;

    eButton::pressed_but_t button = keyboard.get_button();
    

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
            /*
            char timeStr[8]; // Буфер с запасом на 8 байт
            uint8_t hour   = rtc.get_hour();
            uint8_t minute = rtc.get_minute();
            bool comma     = rtc.get_sec_comma();

            // Операции % 24 и % 60 доказывают компилятору, что числа уложатся в 2 знака
            snprintf(timeStr, sizeof(timeStr), "%02d%s%02d", (int)(hour % 24), comma ? ":" : " ", (int)(minute % 60));
            oled.show_time(timeStr);
            */
            oled.show_main_screen(rtc);

            static uint8_t screen = 0;
            switch (screen)
            {
                case 0:
                {
                    displ.show_clock(rtc);
                    
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

            switch(button)
            {
                case eButton::PRESS_UP:
                {
                    if(screen < MAX_COUNT_MENU) ++screen;
                    else screen = 0;
                };break;

                case eButton::PRESS_DOWN:
                {
                    if(screen > 0) --screen;
                    else screen = MAX_COUNT_MENU;
                };break;

                case eButton::PRESS_OK:
                {
                    work_mode = CONFIGURATION;
                };break;

                case eButton::PRESS_CANCEL:
                {

                };break;

                default: break;
            }                   
        }; break;

        case CONFIGURATION:
        {
        
            if(!mainMenu.process(button, ctx)) work_mode = NORMAL_WORK;     

        }; break;

            
    }


}

/************************************************************ */
display_clock_t get_clock(eDS1338& rtc)
{
    display_clock_t clocks;

    clocks.hour = rtc.get_hour();
    clocks.minute = rtc.get_minute();
    clocks.comma = rtc.get_sec_comma();
    return clocks;
}

display_clock_t morning_twilight_start(eDS1338& rtc)
{
    display_clock_t clocks;

    rtc.get_civil_dawn(clocks.hour, clocks.minute);
    clocks.comma = true;
    return clocks;
}

display_clock_t evening_twilight_end(eDS1338& rtc)
{
    display_clock_t clocks;
    
    rtc.get_civil_dusk(clocks.hour, clocks.minute);
    clocks.comma = true;

    return clocks;
}