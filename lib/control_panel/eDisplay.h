#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <time_common.h>
#include <eDS1338.hpp>


#include "bin_bcd.h"


using namespace std;

#include "config_device.h"



constexpr uint8_t STAT              = 0;
constexpr uint8_t BLINK             = 1;
constexpr uint8_t BLINK_HOUR        = 2;
constexpr uint8_t BLINK_MIN         = 3;

/** содержимое одного разряда индикатора */
typedef struct
{
    uint8_t symbol;
    bool comma;

} digit_t;

/** разряды индикатора */
typedef struct
{
    digit_t digit_4;
    digit_t digit_3;
    digit_t digit_2;
    digit_t digit_1;

}display_digit_t;


/** вывод времени */
typedef struct
{
    uint8_t hour;
    uint8_t minute;
    bool comma;

}display_clock_t;





/************************************************************************/
class eDisplay
{
    public:

        eDisplay();

        void display_init(void);
        void display_update(void);
        
        void show(const char str[], uint8_t blink = STAT);
        void show(const uint32_t data);

        void show_clock(const display_clock_t clock, uint8_t blink = STAT);
        void show_clock(eDS1338& rtc, uint8_t blink = STAT);

        void show_day(const uint8_t day, uint8_t blink = STAT);
        void show_date(const uint8_t date, uint8_t blink = STAT);
        void show_month(const uint8_t month, uint8_t blink = STAT);
        void show_year(const uint16_t year, uint8_t blink = STAT);

        void show_location_deg(const int16_t loc, uint8_t blink);
        void show_location_min(const int16_t loc, uint8_t blink);
        void show_location_sec(const int16_t loc, uint8_t blink);
        void show_location_tz(const int8_t time_zone, uint8_t blink);
       

        void display_char_write_pos(uint8_t value, uint8_t pos, uint8_t mode);


    private:
        bcd16_level_t bcd_char;
        display_digit_t digits;

        uint8_t step;
        uint8_t segment;     
        bool blink_stat;
        uint32_t blink;

        uint8_t digit_1_mode;
        uint8_t digit_2_mode;
        uint8_t digit_3_mode;
        uint8_t digit_4_mode;

        void show_location(const int16_t loc, uint8_t param, uint8_t blink);

        void all_digits_stat(void);

};














#endif /* DISPLAY_H_INCLUDED */
