#include "menu_main.h"

//extern eDisplay displ;
//extern eRTC rtc;

using namespace nDS1338;
using Button = eButton::ButtonNumber; 

// Состояния настройки
enum EditTzStep {
    STEP_EDIT_TZ,   
};




bool menu_config_loc_tz(Button button, eMenu::Context& ctx) 
{
    static EditTzStep current_step = STEP_EDIT_TZ;

    // 1. ПРОВЕРКА ОТМЕНЫ (CANCEL) НА САМОМ ВХОДЕ
    // Если нажали CANCEL, сразу выходим, ничего не перерисовывая для этого режима
    if (button == Button::PRESS_CANCEL || button == Button::PRESS_OK_CANCEL) {
        current_step = STEP_EDIT_TZ;     // Сброс шага
        ctx.rtc.rtc_resume();            // Запуск RTC обратно
        return false;                     // Выход в меню
    }

    // 2. АВТОМАТ СОСТОЯНИЙ НАСТРОЙКИ
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

    // 3. ОБНОВЛЕНИЕ ДАННЫХ И ОТРИСОВКА НА ЭКРАНЕ
    // Этот блок гарантированно вызовется, если мы не вышли по CANCEL или OK(на секундах)
    switch(current_step)
    {
        case STEP_EDIT_TZ:
        {
            uint16_t timezone = ctx.rtc.get_timezone();
            ctx.displ.show_location_tz(timezone, BLINK);
            break;
        }

    }

    return true; // Продолжаем оставаться в режиме редактирования долготы
}
