#ifndef EMENU_H_INCLUDED
#define EMENU_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <eButton.h>
#include <eDisplay.h>




class eMenu 
{
    public:
        // Шаблонные типы функций: Display& всегда идет первым, а дальше — что угодно (Args...)
        template <typename... Args>
        using MenuActionFunc = void(*)(uint16_t button, Args... args);

        template <typename... Args>
        using MenuInitFunc = void(*)(Args... args);

        template <typename... Args>
        using MenuTextFunc = void(*)(Display& displ, Args... args);

        // Структура пункта меню хранит универсальные указатели (void*)
        struct Item 
        {
            const char*  label;       // Текстовая строка которая будет выводиться на дисплей 
            const void*  text_func;   // Динамический текст (сиволы, цифры, etc...) который будет выводиться на дисплей 
            const void*  init_func;   // Вызывается строго один раз в момент, когда пользователь нажимает кнопку «ОК» на этом пункте
            eMenu*       submenu;     // Указатель на другой объект типа eMenu (на родительский класс подменю)
            const void*  action_func; // Универсальный указатель (void*) на функцию конечного действия (режима редактирования/выполнения)
        };

    public:
        eMenu(const Item* items, uint8_t size);

        void reset();

        // button, displ обязательные и любое количество других параметров (часы, настройки, датчики...)
        template <typename... Args>
        bool process(uint16_t button, Display& displ, Args&... args) {
            
            // 1. ЕСЛИ АКТИВЕН ВНУТРЕННИЙ ПУНКТ (подменю или действие)
            if (m_active_item != nullptr) {
                // Вариант А: Переход во вложенное подменю
                if (m_active_item->submenu != nullptr) {
                    if (!m_active_item->submenu->process(button, displ, args...)) {
                        m_active_item = nullptr; 
                    }
                    return true;
                }
                
                // Вариант Б: Выполнение конечной функции действия (редактирования)
                if (m_active_item->action_func != nullptr) {
                    if (button == PRESS_CANCEL) {
                        m_active_item = nullptr; 
                    } else {
                        // Строгое приведение к типу функции действия, принимающей (button, displ, args...) по ссылке
                        auto func = reinterpret_cast<void(*)(uint16_t, Display&, Args&...)>(m_active_item->action_func);
                        func(button, displ, args...); 
                    }
                    return true;
                }
            }

            // 2. ОТРИСОВКА ТЕКУЩЕГО ПУНКТА (СТАТИКА ИЛИ ДИНАМИКА)
            const Item& current = m_items[m_current_item];
            
            if (current.text_func != nullptr) {
                // Строгое приведение к типу функции текста, принимающей (displ, args...) по ссылке
                auto func = reinterpret_cast<void(*)(Display&, Args&...)>(current.text_func);
                func(displ, args...); // ИСПРАВЛЕНО: Теперь дисплей передается в функцию текста!
            } else {
                displ.show(current.label);
            }

            // 3. ОБРАБОТКА НАВИГАЦИИ КНОПКАМИ ГЛАВНОГО МЕНЮ
            switch (button) {
                case PRESS_UP:   
                    if (++m_current_item >= m_size) m_current_item = 0; 
                    break;
                    
                case PRESS_DOWN: 
                    if (--m_current_item < 0) m_current_item = m_size - 1; 
                    break;
                    
            case PRESS_OK:
                m_active_item = &m_items[m_current_item];
                
                if (m_active_item->init_func != nullptr) {
                    // ИСПРАВЛЕНО: Явное приведение типа к простой функции void(*)(void) []
                    auto func = reinterpret_cast<void(*)(void)>(m_active_item->init_func);
                    func(); 
                }
                
                if (m_active_item->submenu != nullptr) {
                    m_active_item->submenu->reset();
                }
                break;
                    
                case PRESS_CANCEL: 
                    return false; 
                    
                default: 
                    break;
            }

            return true;
    }

    private:
        const Item*   m_items;        
        const uint8_t m_size;         
        int8_t        m_current_item; 
        const Item*   m_active_item;  
};


#endif // EMENU_H_INCLUDED