#include <eButton.h>


EButton::EButton()
{
    pressed_but = NOT_PRESED;
}

uint16_t EButton::get_button(void)
{
    uint16_t save_presed_but;
    save_presed_but = pressed_but;
    pressed_but = NOT_PRESED;

    return save_presed_but;
}



void EButton::button_update(void)
{

    uint16_t temp_press = NOT_PRESED;

    if((!HAL_GPIO_ReadPin(BUT_OK_PORT, BUT_OK_PIN)) && (!HAL_GPIO_ReadPin(BUT_CANCEL_PORT, BUT_CANCEL_PIN))) temp_press = PRESS_OK_CANCEL;
    else if((!HAL_GPIO_ReadPin(BUT_UP_PORT, BUT_UP_PIN)) && (!HAL_GPIO_ReadPin(BUT_DOWN_PORT, BUT_DOWN_PIN))) temp_press = PRESS_UP_DOWN;
    else if(!HAL_GPIO_ReadPin(BUT_OK_PORT, BUT_OK_PIN)) temp_press = PRESS_OK;
    else if(!HAL_GPIO_ReadPin(BUT_CANCEL_PORT, BUT_CANCEL_PIN)) temp_press = PRESS_CANCEL;
    else if(!HAL_GPIO_ReadPin(BUT_UP_PORT, BUT_UP_PIN)) temp_press = PRESS_UP;
    else if(!HAL_GPIO_ReadPin(BUT_DOWN_PORT, BUT_DOWN_PIN)) temp_press = PRESS_DOWN;
    else temp_press = NOT_PRESED;

    //если во временной переменной что-то есть

    if(temp_press)
    {
        //если кнопка нажата дольше задержки до автоперебора
	    //записать ее номер в буфер
        if(comp > HOLD)
        {
	        comp = (HOLD - BUST);
            if((temp_press == PRESS_UP)||(temp_press == PRESS_DOWN)) pressed_but = temp_press;
	    }
        else comp++;

        //если кнопка нажата дольше антидребезга
	    //записать ее номер в буфер
        if (comp == THRESHOLD)
        {
	        pressed_but = temp_press;
	    }
    }

    else comp = 0;
}

