#include "menu_main.h"


using namespace nDS1338;
using Button = eButton::ButtonNumber; 

// Состояния настройки
enum eEditDateStep {
    STEP_EDIT_YEAR,   
    STEP_EDIT_MONTH,  
    STEP_EDIT_DATE,
};


bool menu_config_rtc_date(Button button, eMenu::Context& ctx)
{
    static eEditDateStep current_step = STEP_EDIT_YEAR;

    switch(current_step)
    {
        case STEP_EDIT_YEAR: 
        {
            if (button == Button::PRESS_UP)     { ctx.rtc.change_parameter(Parameter::YEAR, TypeOp::PLUS); }
            if (button == Button::PRESS_DOWN)   { ctx.rtc.change_parameter(Parameter::YEAR, TypeOp::MINUS); }
            if (button == Button::PRESS_OK)     { current_step = STEP_EDIT_MONTH; }
        } break;

        case STEP_EDIT_MONTH: 
        {
            if (button == Button::PRESS_UP)     { ctx.rtc.change_parameter(Parameter::MONTH, TypeOp::PLUS); }
            if (button == Button::PRESS_DOWN)   { ctx.rtc.change_parameter(Parameter::MONTH, TypeOp::MINUS); }
            if (button == Button::PRESS_OK)     { current_step = STEP_EDIT_DATE; }
        } break;

        case STEP_EDIT_DATE: 
        {
            if (button == Button::PRESS_UP)     { ctx.rtc.change_parameter(Parameter::DATE, TypeOp::PLUS); }
            if (button == Button::PRESS_DOWN)   { ctx.rtc.change_parameter(Parameter::DATE, TypeOp::MINUS); }
            
            if (button == Button::PRESS_OK) 
            {
                ctx.rtc.change_parameter(Parameter::EMPTY, TypeOp::APPLY_DATE); // Сохраняем
                current_step = STEP_EDIT_YEAR; // Сбрасываем шаги на начало            
                ctx.rtc.rtc_resume();
                return false; 
            }
        } break;
    }

    if (current_step == STEP_EDIT_YEAR) 
    {
        uint16_t year = ctx.rtc.get_year();
        ctx.displ.show_year(year, BLINK); 
    } 
    else if (current_step == STEP_EDIT_MONTH) 
    {
        uint8_t month = ctx.rtc.get_month();
        ctx.displ.show_month(month, BLINK);  
    }
    else if (current_step == STEP_EDIT_DATE) 
    {
        uint8_t date = ctx.rtc.get_date();
        ctx.displ.show_date(date, BLINK);  
    } 

    if (button == Button::PRESS_CANCEL) 
    {
        current_step = STEP_EDIT_YEAR;
        ctx.rtc.rtc_resume();
        return false;
    }

    return true;
}

