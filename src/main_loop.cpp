#include <main_loop.h>

extern eDisplay displ;
extern ELed led_1, led_2, led_3;

extern eDS1338 rtc;
extern eGPS gps;

extern eButton keyboard;
extern eEEPROM eeprom;
extern eChannel channel[CHANNEL_AMOUNT];
extern uint8_t test_month;

display_clock_t get_clock(eDS1338& rtc);
display_clock_t morning_twilight_start(eDS1338& rtc);
display_clock_t evening_twilight_end(eDS1338& rtc);


const uint8_t MAX_COUNT_MENU = 6;

eMenu::Context ctx = { displ, rtc, gps }; 

using Button = eButton::ButtonNumber; 



enum STEP_STARTUP_LOAD
{
    STEP_LOAD_RTC_DATA = 0,
    STEP_LOAD_CH_DATA,
};

void main_loop(void)
{
    static uint8_t work_mode = STARTUP_LOAD;
    static uint8_t startup_load_step = 0;

    Button button = keyboard.get_button();
    

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
                    displ.show_clock(gps);
                };break;

                case 1:
                {
                    displ.show_date(gps.get_date());
                };break;   

                case 2:
                {
                    displ.show_day(gps.get_day());
                };break;

                case 3:
                {
                    displ.show_month(gps.get_month());
                };break;  

                case 4:
                {
                    displ.show_year(gps.get_year());
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
                case Button::PRESS_UP:
                {
                    if(screen < MAX_COUNT_MENU) ++screen;
                    else screen = 0;
                };break;

                case Button::PRESS_DOWN:
                {
                    if(screen > 0) --screen;
                    else screen = MAX_COUNT_MENU;
                };break;

                case Button::PRESS_OK:
                {
                    work_mode = CONFIGURATION;
                };break;

                case Button::PRESS_CANCEL:
                {

                };break;

                default: break;
            }                   
        }; break;

        case CONFIGURATION:
        {
        
            bool menu_active = mainMenu.process(button, ctx);
            button = Button::NOT_PRESSED; 
            if (!menu_active) {
                work_mode = NORMAL_WORK;     
            }

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