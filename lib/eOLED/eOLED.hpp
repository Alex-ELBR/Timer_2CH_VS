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
        void periodic(void);
        void show_time(const char* time);

    private:
        I2C_HandleTypeDef *_i2c_bus;
        uint16_t _dev_address;
        u8g2_t _u8g2; 
        uint8_t _prev_buffer[1024]; 
};


/*
Набор u8g2_font_open_iconic_embedded_2x_t (Размер 16x16)

    74 — Bluetooth 
    80 — Ключ / Настройки (иконка гаечного ключа)
    67 — Батарея (полный заряд)
    68 — Батарея (средний заряд)
    64 — Электросеть / Вилка 
    67 — Спутниковая антенна / Радар 
    68 — Сигнал / Антенна 
    71 — Замок закрыт 
    72 — Замок открыт 
    85 — Восклицательный знак в треугольнике 
    68 — Крестик (X)
    74 — Галочка (V)
*/
#endif //EOLED_H_INCLUDED

