#include "eMenu.h"

eMenu::eMenu(const Item* items, uint8_t size) 
    : m_items(items), m_size(size), m_current_index(0), m_active_item(nullptr) {}

void eMenu::reset() {
    m_current_index = 0;
    m_active_item = nullptr;
}

bool eMenu::process(eButton::pressed_but_t button, Context& ctx) {
    
    // 1. ЕСЛИ АКТИВЕН ВНУТРЕННИЙ ПУНКТ (Подменю или Режим редактирования)
    if (m_active_item != nullptr) {
        
        // Вариант А: Мы находимся внутри вложенного подменю
        if (m_active_item->submenu != nullptr) {
            // Передаем кнопку и контекст в подменю
            // Если подменю вернуло false (в нем нажали CANCEL), закрываем его
            if (!m_active_item->submenu->process(button, ctx)) {
                m_active_item = nullptr; 
            }
            return true;
        }
        
        // Вариант Б: Мы находимся в режиме выполнения действия (например, изменение числа)
        if (m_active_item->action_func != nullptr) {
            // Передаем управление и кнопку в функцию действия
            bool exit = m_active_item->action_func(button, ctx);
            
            // Если в режиме редактирования нажали CANCEL или функция вернула false — выходим из этого режима назад в меню
            if (button == eButton::PRESS_CANCEL || button == eButton::PRESS_OK_CANCEL || exit == false) {
                m_active_item = nullptr;
            }
            return true;
        }
    }

    // 2. НАВИГАЦИЯ ПО ТЕКУЩЕМУ МЕНЮ (Если ни один пункт не активен)
    switch (button) {
        case eButton::PRESS_DOWN:
            m_current_index++;
            if (m_current_index >= m_size) {
                m_current_index = 0; // Зацикливание списка вниз
            }
            break;

        case eButton::PRESS_UP:
            if (m_current_index == 0) {
                m_current_index = m_size - 1; // Зацикливание списка вверх
            } else {
                m_current_index--;
            }
            break;

        case eButton::PRESS_OK: {
            const Item* selected = &m_items[m_current_index];
            
            // Если у пункта есть функция инициализации — вызываем один раз перед входом
            if (selected->init_func != nullptr) {
                selected->init_func(ctx);
            }

            // Активируем подменю или функцию действия
            if (selected->submenu != nullptr || selected->action_func != nullptr) {
                m_active_item = selected;
                
                // Если это подменю, сбрасываем его указатель в начало
                if (selected->submenu != nullptr) {
                    selected->submenu->reset();
                }
            }
            break;
        }

        case eButton::PRESS_CANCEL:
            // Возвращаем false, сигнализируя родительскому меню, что из текущего нужно выйти
            return false; 

        default:
            break;
    }

    // 3. ОТРИСОВКА МЕНЮ
    // Здесь должна быть ваша функция отрисовки экрана, например:
    // Display_Draw_Menu(m_items, m_size, m_current_index, ctx);
    // Внутри нее для каждого пункта можно вызывать m_items[i].text_func(ctx),
    // чтобы вывести изменяющиеся значения (например, "Громкость: 45%")

    const Item* current = &m_items[m_current_index];
    ctx.displ.show(current->label);

    return true; // Остаемся в текущем меню
}
