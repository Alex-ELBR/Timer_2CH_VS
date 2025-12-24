#ifndef EBUTTON_H_INCLUDED
#define EBUTTON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"

enum _BUTTON_NUMBER__
{
    NOT_PRESED,
    PRESS_CANCEL,        
    PRESS_UP,           
    PRESS_DOWN,          
    PRESS_OK,            
    PRESS_UP_DOWN,       
    PRESS_OK_CANCEL,        
};


class EButton
{
    public:
        EButton();

        void button_update(void);
        uint16_t get_button(void);

    private:
        uint16_t pressed_but;
        uint32_t comp;

};


#endif //EBUTTON_H_INCLUDED