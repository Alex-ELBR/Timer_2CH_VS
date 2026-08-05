#ifndef EDISPLAY_H_INCLUDED
#define EDISPLAY_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include "stm32f1xx_hal.h"
#include <eDS1338.hpp>
#include "config_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "u8g2_core/u8g2.h" 

#ifdef __cplusplus
}
#endif


class eDisplay
{
    public:
        eDisplay(I2C_HandleTypeDef *i2c_obj, uint16_t address);
        void init(void);
        void set_max_brightness(void);
        void periodic(void);
        void show_main_screen(eDS1338& rtc);
        void show_other_display(void);
        void clear(void);

    private:
        I2C_HandleTypeDef *_i2c_bus;
        uint16_t _dev_address;
        u8g2_t _u8g2; 
        uint8_t _prev_buffer[1024]; 
        bool _is_already_max{false};
        uint32_t _time_high_brightness{0};

        void _high_brightness(void);
        void _low_brightness(void);

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
#endif //EDISPLAY_H_INCLUDED

