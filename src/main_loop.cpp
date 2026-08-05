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

// Перегрузка ++screen
ScreensNormalWork& operator++(ScreensNormalWork& screen) {
    uint8_t next = static_cast<uint8_t>(screen) + 1;
    
    // Если вышли за пределы последнего экрана, сбрасываем на первый (0)
    if (next >= static_cast<uint8_t>(ScreensNormalWork::COUNT)) {
        screen = static_cast<ScreensNormalWork>(0);
    } else {
        screen = static_cast<ScreensNormalWork>(next);
    }
    return screen;
}

// Перегрузка --screen
ScreensNormalWork& operator--(ScreensNormalWork& screen) {
    uint8_t current = static_cast<uint8_t>(screen);
    
    // Если мы на первом экране (0), декремент переводит на самый последний экран
    if (current == 0) {
        screen = static_cast<ScreensNormalWork>(static_cast<uint8_t>(ScreensNormalWork::COUNT) - 1);
    } else {
        screen = static_cast<ScreensNormalWork>(current - 1);
    }
    return screen;
}

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
            static ScreensNormalWork screen = ScreensNormalWork::MAIN_SCREEN;

            switch (screen)
            {
                case ScreensNormalWork::MAIN_SCREEN:
                {
                    displ.show_main_screen(rtc);
                };break;

                case ScreensNormalWork::OTHER_SCREEN:
                {
                    displ.clear();
                };break;   

                default:{ break; }
            }

            switch(button)
            {
                case eButton::PRESS_UP:
                {
                    ++screen;
                };break;

                case eButton::PRESS_DOWN:
                {
                    --screen;
                };break;

                case eButton::PRESS_OK:
                {
                    work_mode = WorkMode::CONFIGURATION;
                };break;

                case eButton::PRESS_CANCEL:
                {
                    screen = ScreensNormalWork::MAIN_SCREEN;
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


