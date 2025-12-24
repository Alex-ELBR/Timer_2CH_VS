#ifndef __ELED_H
#define __ELED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"


class ELed
{
    private:
        uint8_t state_led;

        bool toggle_led;

        GPIO_TypeDef *eled_port;
        uint16_t eled_pin;


    public:
        ELed(GPIO_TypeDef *port, uint16_t pin);
        void on(void);
        void off(void);
        void blink(void);
        void toogle(void);

        void periodic(void);




};



#endif //__ELED_H