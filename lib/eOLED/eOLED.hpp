#ifndef EOLED_H_INCLUDED
#define EOLED_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include "stm32f1xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "u8g2_core/u8g2.h" 

#ifdef __cplusplus
}
#endif

class eOLED
{
    public:
        eOLED(I2C_HandleTypeDef *i2c_obj, uint16_t address);
        void init(void);
        void updateScreen(const char* date, const char* time, const char* tw_start, const char* tw_end);

    private:
        I2C_HandleTypeDef *_i2c_bus;
        uint16_t _dev_address;
        u8g2_t _u8g2; // Структура библиотеки u8g2 под капотом класса
};

#endif //EOLED_H_INCLUDED
