#include <main_loop.h>

extern eDisplay displ;
extern ELed led_1, led_2, led_3;
extern eDS1338 rtc;
extern eButton keyboard;
extern eEEPROM eeprom;
extern eChannel channel[CHANNEL_AMOUNT];
extern uint8_t test_month;

const uint8_t MAX_COUNT_MENU = 6;

eMenu::Context ctx = { displ, rtc }; 

enum class StartupLoadStep : uint8_t 
{
    STEP_LOAD_RTC_DATA,
    STEP_LOAD_CH_DATA,
};



void main_loop(void)
{
    static WorkMode work_mode = WorkMode::STARTUP_LOAD;
    static StartupLoadStep startup_load_step = StartupLoadStep::STEP_LOAD_RTC_DATA;
    static eButton::pressed_but_t prev_button = eButton::NOT_PRESSED;

    eButton::pressed_but_t button = keyboard.get_button();

    if (button != eButton::NOT_PRESSED && prev_button == eButton::NOT_PRESSED) {
        displ.set_max_brightness();
    }

    prev_button = button;

    switch(work_mode)
    {
    /************************************************/
        case WorkMode::STARTUP_LOAD:
        {
            switch(startup_load_step)
            {
                case StartupLoadStep::STEP_LOAD_RTC_DATA:
                {
                    led_1.on();
                    startup_load_step = StartupLoadStep::STEP_LOAD_CH_DATA;
                }; break;

                case StartupLoadStep::STEP_LOAD_CH_DATA:
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
                        work_mode = WorkMode::NORMAL_WORK;
                        startup_load_step = StartupLoadStep::STEP_LOAD_RTC_DATA;
                    }
                }; break;
               
            }

        }; break;
    /************************************************/
        case WorkMode::NORMAL_WORK:
        {
            static uint8_t screen = 0;
            switch (screen)
            {
                case 0:
                {
                    displ.show_main_screen(rtc);
                };break;

                case 1:
                {

                };break;   

                case 2:
                {
                    
                };break;

                case 3:
                {

                };break;  

                case 4:
                {

                };break;

                case 5:
                {

                };break;
                
                case 6:
                {

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
                    work_mode = WorkMode::CONFIGURATION;
                };break;

                case eButton::PRESS_CANCEL:
                {
                    screen = 0;
                };break;

                default: break;
            }                   
        }; break;

        case WorkMode::CONFIGURATION:
        {
        
            if(!mainMenu.process(button, ctx)) work_mode = WorkMode::NORMAL_WORK;     

        }; break;

            
    }


}


