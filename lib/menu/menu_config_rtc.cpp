#include "menu_config_rtc.h"

extern Display displ;
extern eRTC rtc;


bool menu_config_rtc(uint16_t button)
{
    // step_config остается static, чтобы сохранять шаг настройки между тактами опроса кнопок
    static uint8_t step_config = 0; 
    
    // change_parameter ОБЯЗАН быть локальным, чтобы рассчитываться заново каждый такт!
    uint8_t change_parameter = CHANGE_HOUR; 

    display_clock_t clocks;
    clocks.hour = rtc.get_hour();
    clocks.minute = rtc.get_minute();
    clocks.comma = true;

    // --- 1. ОБРАБОТКА КНОПОК ---
    switch(button)
    {
        case PRESS_CANCEL:
            rtc.stop_change();
            step_config = 0; // Сбрасываем шаг для следующего входа в меню
            return false;    // Команда для menu_main: "мы выходим"

        case PRESS_UP:
            // Сначала определяем, какой параметр активен на этом шаге
            // (это произойдет ниже в switch(step_config), поэтому перенесем расчет параметра перед кнопками)
            break;

        case PRESS_DOWN:
            break;

        case PRESS_OK:
            if (step_config < STEP_APPLY_CONFIG_RTC - 1) 
            {
                ++step_config;
            }
            else
            {
                rtc.change_parameter(0, APPLY_RTC); // Запись в RTC по I2C
                step_config = 0;                    // Обнуляем статический шаг!
                return false;                       // Выходим в главное меню
            }
            break;
            
        default:
            break;
    }   

    // --- 2. ОПРЕДЕЛЕНИЕ ТЕКУЩЕГО ПАРАМЕТРА И ОТРИСОВКА ---
    switch(step_config)
    {
        case STEP_CONFIG_HOUR:
            change_parameter = CHANGE_HOUR;
            displ.show_clock(clocks, BLINK_HOUR);
            break;

        case STEP_CONFIG_MIN:
            change_parameter = CHANGE_MINUTE;
            displ.show_clock(clocks, BLINK_MIN);
            break;

        case STEP_CONFIG_YEAR:
            change_parameter = CHANGE_YEAR;
            displ.show_year(rtc.get_year(), BLINK);
            break; 

        case STEP_CONFIG_MONTH:
            change_parameter = CHANGE_MONTH;
            displ.show_month(rtc.get_month(), BLINK);
            break;

        case STEP_CONFIG_DATE:
            change_parameter = CHANGE_DATE;
            displ.show_date(rtc.get_date(), BLINK);
            break;
            
        default:
            break;
    }

    // --- 3. ВЫПОЛНЕНИЕ ИЗМЕНЕНИЯ (если были нажаты стрелки) ---
    if (button == PRESS_UP) 
    {
        rtc.change_parameter(change_parameter, PLUS);
    }
    else if (button == PRESS_DOWN) 
    {
        rtc.change_parameter(change_parameter, MINUS);
    }
    
    return true; // Продолжаем оставаться внутри подменю RTC
}


