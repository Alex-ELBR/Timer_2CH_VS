#ifndef EMENU_H_INCLUDED
#define EMENU_H_INCLUDED

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <eButton.h>
#include <eDisplay.h>
#include <eDS1338.hpp>
#include <eOLED.hpp>



#pragma once
#include <stdint.h>
#include "eButton.h" // Подключаем класс кнопок

// Предварительное объявление класса дисплея (замените на ваш реальный класс, например Adafruit_SSD1306)
class eDisplay; 
class eDS1338;

class eMenu 
{
    public:
        // Структура контекста для передачи сквозных данных во все функции
        struct Context {
            eDisplay& displ;
            eDS1338& rtc;
            eOLED& oled;
            // Config& cfg;
        };

        // Сигнатуры функций теперь строго используют типы из eButton
        using MenuActionFunc = bool(*)(eButton::pressed_but_t button, Context& ctx);
        using MenuInitFunc   = void(*)(Context& ctx);
        using MenuTextFunc   = void(*)(Context& ctx); // Функция для отрисовки динамических значений

        struct Item 
        {
            const char*    label;       // Статический текст пункта
            MenuTextFunc   text_func;   // Функция вывода динамического текста (значений параметров)
            MenuInitFunc   init_func;   // Функция, вызываемая один раз при нажатии OK
            eMenu*         submenu;     // Указатель на подменю
            MenuActionFunc action_func; // Функция режима редактирования/выполнения
        };

    public:
        // Конструктор принимает массив пунктов и его размер
        eMenu(const Item* items, uint8_t size);

        // Сброс состояния меню на начальное
        void reset();

        // Главный метод обработки логики
        bool process(eButton::pressed_but_t button, Context& ctx);

    private:
        const Item* m_items;
        uint8_t     m_size;
        uint8_t     m_current_index;
        const Item* m_active_item;
};


#endif // EMENU_H_INCLUDED