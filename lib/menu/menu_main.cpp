#include "menu_main.h"
#include "eDS1338.hpp" 
#include "eDisplay.h"
#include "eMenu.h"



extern eDisplay displ;
extern eDS1338 rtc;





/** Подменю настройки локации *************************************************************/

eMenu::Item menu_loc_items[] = {
    
    { "LOn ", nullptr, start_config_rtc, nullptr, menu_config_loc_lon },
    { "LAt ", nullptr, start_config_rtc, nullptr, menu_config_loc_lat },
    { "UtC ", nullptr, start_config_rtc, nullptr, menu_config_loc_tz },
};

eMenu locationMenu(menu_loc_items, sizeof(menu_loc_items) / sizeof(menu_loc_items[0]));
/**************************************************************************************** */

/** Главное меню **************************************************************************/
eMenu::Item main_items[] = {
    
    { "Hour", nullptr, start_config_rtc, nullptr, menu_config_rtc }, 
    { "dAtE", nullptr, nullptr, nullptr, nullptr },
    { "Loc ", nullptr, nullptr, &locationMenu, nullptr }, //Подменю настройки локации
    { "Func", nullptr, nullptr, nullptr, nullptr }
};

eMenu mainMenu(main_items, sizeof(main_items) / sizeof(main_items[0]));
/**************************************************************************************** */






void start_config_rtc(eMenu::Context& ctx)
{
   ctx.rtc.rtc_suspend();
}












/*
extern eDS1338 rtc;

// Определяем типы для читаемости кода
using MenuVoidFunc = void(*)(void); // Функция инициализации (ничего не принимает, ничего не возвращает)
using MenuSubFunc = bool(*)(uint16_t, Display&, eDS1338&); // Функция подменю (принимает кнопку, возвращает bool)

struct MenuItem {
    const char*  label;       // Текст для дисплея
    MenuVoidFunc init_func;   // Функция подготовки (вызывается 1 раз при входе)
    MenuSubFunc  config_func; // Основная функция подменю
};

// Объявляем заглушки функций, если они объявлены ниже по коду
bool menu_config_rtc(uint16_t button, Display& displ, eDS1338& rtc);
bool menu_config_loc(uint16_t button, Display& displ, eDS1338& rtc);
bool menu_config_chnl(uint16_t button, Display& displ, eDS1338& rtc);

// Обертка для rtc.start_change(), так как это метод класса, а не обычная функция
void rtc_init_wrapper() { 
    rtc.start_change(); 
}

// Создаем таблицу меню во Flash-памяти (const)
const MenuItem MAIN_MENU[] = {
    // Название | Что сделать при входе | Куда передавать кнопки
    {"hour",    rtc_init_wrapper,     menu_config_rtc},
    {"Loc ",    nullptr,              menu_config_loc}, // nullptr означает "ничего делать не нужно"
    {"CHnL",    nullptr,              menu_config_chnl}
};

// Автоматически вычисляем количество пунктов в меню
constexpr uint8_t MENU_SIZE = sizeof(MAIN_MENU) / sizeof(MAIN_MENU[0]);


bool menu_main(uint16_t button, Display& displ, eDS1338& rtc)
{
    static int8_t current_item = 0; 
    static bool in_submenu = false; 

    // 1. НАХОДИМСЯ ВНУТРИ ПОДМЕНЮ
    if (in_submenu) {
        // Передаем кнопку в подменю. Если оно вернуло false — выходим обратно
        if (!MAIN_MENU[current_item].config_func(button, displ, rtc)) {
            in_submenu = false;
        }
        return true; 
    }

    // 2. ОТРИСОВКА ГЛАВНОГО ЭКРАНА СЛЕДУЮЩЕГО ПУНКТА
    displ.show(MAIN_MENU[current_item].label); 

    // 3. ОБРАБОТКА КНОПОК НАВИГАЦИИ
    switch(button)
    {
        case PRESS_UP:
            if (++current_item >= MENU_SIZE) current_item = 0;
            break;

        case PRESS_DOWN:
            if (--current_item < 0) current_item = MENU_SIZE - 1;
            break;

        case PRESS_OK:
            // Если у пункта меню есть функция инициализации — вызываем её строго 1 раз
            if (MAIN_MENU[current_item].init_func != nullptr) {
                MAIN_MENU[current_item].init_func();
            }
            in_submenu = true; // Взводим флаг перехода в подменю
            break;

        case PRESS_CANCEL:
            return false; // Выход из меню на рабочий экран устройства
            
        default:
            break;
    }

    return true;
}

*/