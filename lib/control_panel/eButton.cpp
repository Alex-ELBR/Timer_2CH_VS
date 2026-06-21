#include "eButton.h"


eButton::eButton()
{
    pressed_but = eButton::NOT_PRESSED;
    comp = 0; 
}

eButton::pressed_but_t eButton::get_button(void)
{
    pressed_but_t save_pressed_but;
    save_pressed_but = pressed_but;
    pressed_but = eButton::NOT_PRESSED;

    return save_pressed_but;
}

void eButton::button_update(void)
{
    pressed_but_t temp_press = eButton::NOT_PRESSED;

    // Считывание физического состояния выводов (активный уровень — LOW)
    if((!HAL_GPIO_ReadPin(BUT_OK_PORT, BUT_OK_PIN)) && (!HAL_GPIO_ReadPin(BUT_CANCEL_PORT, BUT_CANCEL_PIN))) temp_press = eButton::PRESS_OK_CANCEL;
    else if((!HAL_GPIO_ReadPin(BUT_UP_PORT, BUT_UP_PIN)) && (!HAL_GPIO_ReadPin(BUT_DOWN_PORT, BUT_DOWN_PIN))) temp_press = eButton::PRESS_UP_DOWN;
    else if(!HAL_GPIO_ReadPin(BUT_OK_PORT, BUT_OK_PIN)) temp_press = eButton::PRESS_OK;
    else if(!HAL_GPIO_ReadPin(BUT_CANCEL_PORT, BUT_CANCEL_PIN)) temp_press = eButton::PRESS_CANCEL;
    else if(!HAL_GPIO_ReadPin(BUT_UP_PORT, BUT_UP_PIN)) temp_press = eButton::PRESS_UP;
    else if(!HAL_GPIO_ReadPin(BUT_DOWN_PORT, BUT_DOWN_PIN)) temp_press = eButton::PRESS_DOWN;
    else temp_press = eButton::NOT_PRESSED;

    // Если какая-то кнопка нажата
    if(temp_press != eButton::NOT_PRESSED)
    {
        comp++; // Сначала инкрементируем счетчик для текущего цикла

        // 1. Фиксация одиночного нажатия (после антидребезга)
        if (comp == THRESHOLD)
        {
            pressed_but = temp_press;
        }
        
        // 2. Логика автоперебора при удержании кнопки
        // ИСПРАВЛЕНО: В вашем коде условие (comp > HOLD) никогда бы не выполнилось,
        // так как блок "else comp++" увеличивал счетчик только ДО тех пор, пока comp <= HOLD.
        // Но как только comp становился больше HOLD, ветка else переставала работать, 
        // и comp застревал на одном месте, ломая повторные срабатывания.
        if (comp > HOLD)
        {
            // Сбрасываем счетчик назад на величину периода автоповтора
            comp = (HOLD - BUST); 
            
            // Автоперебор разрешен только для кнопок НАВЕРХ и ВНИЗ
            if((temp_press == eButton::PRESS_UP) || (temp_press == eButton::PRESS_DOWN)) 
            {
                pressed_but = temp_press;
            }
        }
    }
    else 
    {
        comp = 0; // Кнопка отпущена — сбрасываем счетчик времени
    }
}
