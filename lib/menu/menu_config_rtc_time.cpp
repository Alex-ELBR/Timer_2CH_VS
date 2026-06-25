#include "menu_main.h"


using namespace nDS1338;
using Button = eButton::ButtonNumber; 

// Состояния настройки
enum eEditStep {
    STEP_EDIT_HOURS,   
    STEP_EDIT_MINUTES,  
};


bool menu_config_rtc_time(Button button, eMenu::Context& ctx)
{
    static eEditStep current_step = STEP_EDIT_HOURS;


    switch(current_step)
    {
        case STEP_EDIT_HOURS: 
        {
            if (button == Button::PRESS_UP) {
                ctx.rtc.change_parameter(Parameter::HOUR, TypeOp::PLUS);
            }
            if (button == Button::PRESS_DOWN) {
                ctx.rtc.change_parameter(Parameter::HOUR, TypeOp::MINUS);
            }
            
            if (button == Button::PRESS_OK) {
                current_step = STEP_EDIT_MINUTES;
            }
        } break;

        case STEP_EDIT_MINUTES: 
        {
            if (button == Button::PRESS_UP) {
                ctx.rtc.change_parameter(Parameter::MINUTE, TypeOp::PLUS);
            }
            if (button == Button::PRESS_DOWN) {
                ctx.rtc.change_parameter(Parameter::MINUTE, TypeOp::MINUS);
            }
            
            if (button == Button::PRESS_OK) {
                ctx.rtc.change_parameter(Parameter::EMPTY, TypeOp::APPLY_TIME); // Сохраняем
                current_step = STEP_EDIT_HOURS; // Сбрасываем шаги на начало            
                ctx.rtc.rtc_resume();
                return false; 
            }
        } break;
    }


    display_clock_t edit_clocks;
    edit_clocks.hour   = ctx.rtc.get_hour();   // Берем измененный час
    edit_clocks.minute = ctx.rtc.get_minute(); // Берем измененную минуту
    edit_clocks.comma  = true;

    if (current_step == STEP_EDIT_HOURS) {
        ctx.displ.show_clock(edit_clocks, BLINK_HOUR); // Мигают разряды часов
    } 
    else if (current_step == STEP_EDIT_MINUTES) {
        ctx.displ.show_clock(edit_clocks, BLINK_MIN);  // Мигают разряды минут
    }
    
    if (button == Button::PRESS_CANCEL) {
        current_step = STEP_EDIT_HOURS;
        ctx.rtc.rtc_resume();
        return false;
    }

    return true;
}

