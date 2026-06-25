#ifndef EBUTTON_H_INCLUDED
#define EBUTTON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"



class eButton
{
    public:
        enum class ButtonNumber : uint16_t 
        {
            NOT_PRESSED,     
            PRESS_CANCEL,        
            PRESS_UP,           
            PRESS_DOWN,          
            PRESS_OK,            
            PRESS_UP_DOWN,       
            PRESS_OK_CANCEL        
        };
        
        //using pressed_but_t = ButtonNumber;

    public:
        eButton();
        void button_update(void);
        ButtonNumber get_button(void);

    private:
        ButtonNumber pressed_but;
        uint32_t comp;

};


#endif //EBUTTON_H_INCLUDED