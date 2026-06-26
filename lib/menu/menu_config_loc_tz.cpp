#include "menu_main.h"


using namespace nDS1338;
using Button = eButton::ButtonNumber; 

// Состояния настройки
enum EditTzStep {
    STEP_EDIT_TZ,   
};




bool menu_config_loc_tz(Button button, eMenu::Context& ctx) 
{
    static EditTzStep current_step = STEP_EDIT_TZ;

    if (button == Button::PRESS_CANCEL || button == Button::PRESS_OK_CANCEL) {
        current_step = STEP_EDIT_TZ;     // Сброс шага
        ctx.rtc.rtc_resume();            // Запуск RTC обратно
        return false;                     // Выход в меню
    }

    switch(current_step)
    {
        case STEP_EDIT_TZ: 
            if (button == Button::PRESS_UP)   ctx.rtc.change_parameter(Parameter::TIMEZONE, TypeOp::PLUS);
            if (button == Button::PRESS_DOWN) ctx.rtc.change_parameter(Parameter::TIMEZONE, TypeOp::MINUS);
            if (button == Button::PRESS_OK) {
                ctx.rtc.change_parameter(Parameter::EMPTY, TypeOp::APPLY_TIMEZONE); // Сохраняем данные
                current_step = STEP_EDIT_TZ;                                   // Сброс шага
                ctx.rtc.rtc_resume();                                              // Запуск RTC
                return false;                                                      // Выход в меню
            }
            break;
    }

    if(current_step == STEP_EDIT_TZ)
    {
        uint16_t timezone = ctx.rtc.get_timezone();
        ctx.displ.show_location_tz(timezone, BLINK);
    }

    if (button == Button::PRESS_CANCEL) 
    {
        current_step = STEP_EDIT_TZ;
        ctx.rtc.rtc_resume();
        return false;
    }

    return true; 
}
