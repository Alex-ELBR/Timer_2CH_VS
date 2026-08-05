#include "menu_main.h"


using namespace nDS1338;
using Button = eButton::ButtonNumber; 

// Состояния настройки
enum EditLatStep {
    STEP_EDIT_DEG,   
    STEP_EDIT_MIN,   
    STEP_EDIT_SEC,
};

bool menu_config_loc_lat(Button button, eMenu::Context& ctx) 
{
    static EditLatStep current_step = STEP_EDIT_DEG;

    if (button == Button::PRESS_CANCEL || button == Button::PRESS_OK_CANCEL) {
        current_step = STEP_EDIT_DEG; // Сброс шага
        ctx.rtc.rtc_resume();            // Запуск RTC обратно
        return false;                     // Выход в меню
    }

    switch(current_step)
    {
        case STEP_EDIT_DEG: 
            if (button == Button::PRESS_UP)   ctx.rtc.change_parameter(Parameter::LAT_DEG, TypeOp::PLUS);
            if (button == Button::PRESS_DOWN) ctx.rtc.change_parameter(Parameter::LAT_DEG, TypeOp::MINUS);
            if (button == Button::PRESS_OK)   current_step = STEP_EDIT_MIN;
            break;

        case STEP_EDIT_MIN: 
            if (button == Button::PRESS_UP)   ctx.rtc.change_parameter(Parameter::LAT_MIN, TypeOp::PLUS);
            if (button == Button::PRESS_DOWN) ctx.rtc.change_parameter(Parameter::LAT_MIN, TypeOp::MINUS);
            if (button == Button::PRESS_OK)   current_step = STEP_EDIT_SEC;
            break;

        case STEP_EDIT_SEC: 
            if (button == Button::PRESS_UP)   ctx.rtc.change_parameter(Parameter::LAT_SEC, TypeOp::PLUS);
            if (button == Button::PRESS_DOWN) ctx.rtc.change_parameter(Parameter::LAT_SEC, TypeOp::MINUS);
            if (button == Button::PRESS_OK) {
                ctx.rtc.change_parameter(Parameter::EMPTY, TypeOp::APPLY_LATITUDE); // Сохраняем данные
                current_step = STEP_EDIT_DEG;                                   // Сброс шага
                ctx.rtc.rtc_resume();                                              // Запуск RTC
                return false;                                                      // Выход в меню
            }
            break;
    }

    switch(current_step)
    {
        case STEP_EDIT_DEG:
        {
            uint16_t degree = ctx.rtc.get_latitude_degree();
            ctx.displ.show_location_deg(degree, BLINK);
            break;
        }
        case STEP_EDIT_MIN:
        {
            uint16_t minute = ctx.rtc.get_latitude_min();
            ctx.displ.show_location_min(minute, BLINK); 
            break;
        }
        case STEP_EDIT_SEC:
        {
            uint16_t second = ctx.rtc.get_latitude_sec();
            ctx.displ.show_location_sec(second, BLINK); 
            break;
        }
    }

    return true; 
}
