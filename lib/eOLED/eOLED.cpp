#include "eOLED.hpp"
#include <string.h>

static I2C_HandleTypeDef *current_i2c_bus = NULL;
static uint8_t i2c_buffer[128];
static uint8_t i2c_buf_idx;

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
            if (!current_i2c_bus) return 0;
            
            HAL_I2C_Master_Transmit(current_i2c_bus, u8x8_GetI2CAddress(u8x8), i2c_buffer, i2c_buf_idx, 100);
            break;
        }
        default:
            return 0;
    }
    return 1;
}

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

eOLED::eOLED(I2C_HandleTypeDef *i2c_obj, uint16_t address) 
    : _i2c_bus(i2c_obj), _dev_address(address) {
}

void eOLED::init(void) {
    current_i2c_bus = _i2c_bus;

    u8g2_Setup_ssd1306_i2c_128x64_noname_f(
        &_u8g2, 
        U8G2_R0, 
        u8x8_byte_stm32_hw_i2c, 
        u8x8_gpio_and_delay_stm32
    );

    memset(_prev_buffer, 0, sizeof(_prev_buffer));

    // Устанавливаем 8-битный адрес экрана (сдвиг влево на 1 бит)
    u8x8_SetI2CAddress(&_u8g2.u8x8, _dev_address << 1);
    
    u8g2_InitDisplay(&_u8g2);
    u8g2_SetPowerSave(&_u8g2, 0); // Включаем экран
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void eOLED::periodic(void){ 

    uint8_t *current_buffer = u8g2_GetBufferPtr(&_u8g2);

    // Если изменений в буфере не было
    if (memcmp(current_buffer, _prev_buffer, 1024) == 0) {
        return;  
    }

    u8g2_SendBuffer(&_u8g2); 
    memcpy(_prev_buffer, current_buffer, 1024);
}


void eOLED::show_time(const char* time) {
    u8g2_ClearBuffer(&_u8g2);

    // размер 6x10, всегда моноширинный
    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&_u8g2, 34, 10, "24.07.2026");

    char hours_buf[3] = { time[0], time[1], '\0' };
    char minutes_buf[3] = { time[3], time[4], '\0' };
    
    bool is_separator_visible = (time[2] == ':');

    u8g2_SetFont(&_u8g2, u8g2_font_fub20_tn);

    u8g2_DrawStr(&_u8g2, 24, 38, hours_buf);     // Часы всегда начинаются на X = 24
    u8g2_DrawStr(&_u8g2, 74, 38, minutes_buf);   // Минуты НАМЕРТВО зафиксированы на X = 74

    if (is_separator_visible) {
        // Верхняя точка двоеточия (координаты X, Y, ширина, высота)
        u8g2_DrawBox(&_u8g2, 64, 23, 3, 3);
        // Нижняя точка двоеточия
        u8g2_DrawBox(&_u8g2, 64, 31, 3, 3);
    }

    u8g2_SetFont(&_u8g2, u8g2_font_open_iconic_weather_2x_t);
    u8g2_DrawGlyph(&_u8g2, 10, 62, 66); // Луна
    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&_u8g2, 32, 58, "21:48");

    u8g2_SetFont(&_u8g2, u8g2_font_open_iconic_weather_2x_t);
    u8g2_DrawGlyph(&_u8g2, 75, 62, 69); // Солнце
    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&_u8g2, 97, 58, "04:12");

}

 void eOLED::show_main_screen(eDS1338& rtc)
 {
    u8g2_ClearBuffer(&_u8g2);


    // ОТОБРАЖЕНИЕ ВРЕМЕНИ
    char timeStr[8]; // Буфер с запасом на 8 байт
    uint8_t hour   = rtc.get_hour();
    uint8_t minute = rtc.get_minute();
    bool comma     = rtc.get_sec_comma();
    snprintf(timeStr, sizeof(timeStr), "%02d%s%02d", (int)(hour % 24), comma ? ":" : " ", (int)(minute % 60));
    char hours_buf[3] = { timeStr[0], timeStr[1], '\0' };
    char minutes_buf[3] = { timeStr[3], timeStr[4], '\0' };
    bool is_separator_visible = (timeStr[2] == ':');

    u8g2_SetFont(&_u8g2, u8g2_font_fub20_tn);

    u8g2_DrawStr(&_u8g2, 24, 38, hours_buf);     
    u8g2_DrawStr(&_u8g2, 70, 38, minutes_buf);   

    if (is_separator_visible) {
        u8g2_DrawBox(&_u8g2, 60, 23, 3, 3);
        u8g2_DrawBox(&_u8g2, 60, 31, 3, 3);
    }

    //ОТОБРАЖЕНИЕ ДАТЫ
    uint8_t date = rtc.get_date();
    uint8_t month = rtc.get_month();
    uint16_t year = rtc.get_year();
    char dateStr[12]; 

    snprintf(dateStr, sizeof(dateStr), "%02d.%02d.%04d", 
            (int)(date % 32), 
            (int)(month % 13), 
            (int)(year % 10000));

    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&_u8g2, 34, 10, dateStr); 

    // ОТОБРАЖЕНИЕ ВРЕМЕНИ НАЧАЛА/КОНЦА ГРАЖДАНСКИХ СУМЕРЕК
    char civilStr[8]; // Буфер с запасом на 8 байт
    uint8_t civilHour   = 0;
    uint8_t civilMinute = 0;

    rtc.get_civil_dawn(civilHour, civilMinute);
    snprintf(civilStr, sizeof(civilStr), "%02d:%02d", (int)(civilHour % 24), (int)(civilMinute % 60));
    u8g2_SetFont(&_u8g2, u8g2_font_open_iconic_weather_2x_t);
    u8g2_DrawGlyph(&_u8g2, 10, 62, 66); // Луна
    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&_u8g2, 32, 58, civilStr);

    rtc.get_civil_dusk(civilHour, minute);
    snprintf(civilStr, sizeof(civilStr), "%02d:%02d", (int)(civilHour % 24), (int)(civilMinute % 60));
    u8g2_SetFont(&_u8g2, u8g2_font_open_iconic_weather_2x_t);
    u8g2_DrawGlyph(&_u8g2, 75, 62, 69); // Солнце
    u8g2_SetFont(&_u8g2, u8g2_font_6x10_tf);
    u8g2_DrawStr(&_u8g2, 97, 58, civilStr);

}