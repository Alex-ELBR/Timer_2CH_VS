#include "eOLED.hpp"
#include <string.h>

// Статический Си-указатель на I2C шину. 
// Он свяжет изолированный Си-коллбэк с динамическим адресом из вашего C++ класса.
static I2C_HandleTypeDef *current_i2c_bus = NULL;

// Статические буферы для накопления байт перед отправкой пакета по I2C
static uint8_t i2c_buffer[128];
static uint8_t i2c_buf_idx;

// Низкоуровневая Си-функция отправки данных, полностью очищенная от макросов user_ptr
static uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch(msg) {
        case U8X8_MSG_BYTE_INIT:
            break;
        case U8X8_MSG_BYTE_SET_DC:
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            i2c_buf_idx = 0;
            break;
        case U8X8_MSG_BYTE_SEND:
            if (i2c_buf_idx + arg_int < 128) {
                memcpy(&i2c_buffer[i2c_buf_idx], arg_ptr, arg_int);
                i2c_buf_idx += arg_int;
            }
            break;
        case U8X8_MSG_BYTE_END_TRANSFER: {
            // Если шина не привязана, выходим во избежание HardFault
            if (!current_i2c_bus) return 0;
            
            // Прямая отправка через сохраненный адрес шины и макрос адреса дисплея
            HAL_I2C_Master_Transmit(current_i2c_bus, u8x8_GetI2CAddress(u8x8), i2c_buffer, i2c_buf_idx, 100);
            break;
        }
        default:
            return 0;
    }
    return 1;
}

// Коллбэк временных задержек под STM32 HAL
static uint8_t u8x8_gpio_and_delay_stm32(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch(msg) {
        case U8X8_MSG_DELAY_MILLI:
            HAL_Delay(arg_int);
            break;
        default:
            return 0;
    }
    return 1;
}

// Конструктор класса
eOLED::eOLED(I2C_HandleTypeDef *i2c_obj, uint16_t address) 
    : _i2c_bus(i2c_obj), _dev_address(address) {
}

// Функция инициализации дисплея
void eOLED::init(void) {
    // Сохраняем переданный указатель шины I2C в наш статический Си-контекст перед инициализацией
    current_i2c_bus = _i2c_bus;

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
        &_u8g2, 
        U8G2_R0, 
        u8x8_byte_stm32_hw_i2c, 
        u8x8_gpio_and_delay_stm32
    );

    // Устанавливаем 8-битный адрес экрана (сдвиг влево на 1 бит)
    u8x8_SetI2CAddress(&_u8g2.u8x8, _dev_address << 1);
    
    // Аппаратный запуск дисплея
    u8g2_InitDisplay(&_u8g2);
    u8g2_SetPowerSave(&_u8g2, 0); // Включаем экран
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void eOLED::periodic(void){ u8g2_SendBuffer(&_u8g2); }


// Отрисовка вашего экрана сумерек
void eOLED::updateScreen(const char* date, const char* time, const char* tw_start, const char* tw_end) {
    u8g2_ClearBuffer(&_u8g2);

    // 1. ДАТА (Сверху по центру)
    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&_u8g2, 34, 10, date);

    // 2. ВРЕМЯ (Крупный шрифт по центру)
    u8g2_SetFont(&_u8g2, u8g2_font_fub20_tn);
    u8g2_DrawStr(&_u8g2, 24, 38, time);

    // 3. НАЧАЛО СУМЕРЕК (Символ месяца)
    u8g2_SetFont(&_u8g2, u8g2_font_open_iconic_weather_2x_t);
    // ИСПРАВЛЕНО: __u8g2 заменена на _u8g2
    u8g2_DrawGlyph(&_u8g2, 10, 62, 66); // 66 - код Луны
    
    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&_u8g2, 32, 58, tw_start);

    // 4. КОНЕЦ СУМЕРЕК (Символ солнца)
    u8g2_SetFont(&_u8g2, u8g2_font_open_iconic_weather_2x_t);
    u8g2_DrawGlyph(&_u8g2, 75, 62, 69); // 69 - код Солнца

    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    // ИСПРАВЛЕНО: __u8g2 заменена на _u8g2
    u8g2_DrawStr(&_u8g2, 97, 58, tw_end);

}
