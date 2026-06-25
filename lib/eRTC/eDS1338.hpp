#ifndef EDS1338_H_INCLUDED
#define EDS1338_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstdint>
#include "stm32f1xx_hal.h"
#include <time_common.h>
#include <twilight.hpp>


namespace nDS1338{

    // типы для параметра "op" функции change_parameter() 
    enum class TypeOp : std::uint8_t
    {
        PLUS,
        MINUS,
        APPLY_TIME,
        APPLY_DATE,
        APPLY_LONGITUDE,
        APPLY_LATITUDE,
        APPLY_TIMEZONE
    };
    
    // типы для параметра "parameter_name" функции change_parameter()
    enum class Parameter : std::uint8_t
    {
        EMPTY,
        HOUR,
        MINUTE,
        DAY,
        DATE,
        MONTH,
        YEAR,

        LON_DEG,
        LON_MIN,
        LON_SEC,
        LAT_DEG,
        LAT_MIN,
        LAT_SEC,
        TIMEZONE
    };
}







//размещение данных в микросхеме RTC DS1338
#pragma pack(push, 1)
typedef struct 
{
    uint8_t  second;          /**< секунды           - [ 0 to 59 ] */
    uint8_t  minute;          /**< минуты            - [ 0 to 59 ] */
    uint8_t  hour;            /**< часы              - [ 0 to 23 ] */
    uint8_t  day;             /**< день недели       - [ 1 to 7 ]  */
    uint8_t  date;            /**< число календарное - [ 1 to 31 ] */
    uint8_t  month;           /**< месяц             - [ 1 to 12 ] */
    uint8_t  year;            /**< год               - [ 00 - 99 ] */
    uint8_t  control;
    float    latitude;        /**< географическая широта           */
    float    longitude;       /**< географическая долгота          */
    float    time_zone;       /**< часовой пояс                    */
   
} ds1338_data_t;
#pragma pack(pop)



using namespace nDS1338;
/********************************************************************************************* */
class eDS1338
{
    public:
        eDS1338(I2C_HandleTypeDef *i2c_obj, uint16_t address);

        HAL_StatusTypeDef periodic(void);

        /* Операции чтения данных часов */
        uint32_t get_unix_time(void);
        uint8_t get_hour(void);
        uint8_t get_minute(void);
        bool get_sec_comma(void);
        uint8_t get_day(void);
        uint8_t get_date(void);
        uint8_t get_month(void);
        uint16_t get_year(void);
        
        uint32_t get_sec_only_day(void);
        uint32_t get_sec_week(void);

        int16_t get_latitude_degree(void);
        int16_t get_latitude_min(void);
        int16_t get_latitude_sec(void);
        int16_t get_longitude_degree(void);
        int16_t get_longitude_min(void);
        int16_t get_longitude_sec(void);
        int16_t get_timezone(void);

        /* Операции чтения Астрономических данных */
        void get_civil_dawn(uint8_t &hour, uint8_t &minute); // Начало утренних гражданских сумерек
        void get_civil_dusk(uint8_t &hour, uint8_t &minute); // Конец вечерних гражданских сумерек

        /* Операции изменения данных часов */
        void rtc_suspend(void);
        void rtc_resume(void);
        HAL_StatusTypeDef change_parameter(Parameter parameter_name, TypeOp op);


    private:
        I2C_HandleTypeDef *_i2c_bus;
        uint16_t _dev_address;
        uint8_t _rtc_buffer[sizeof(ds1338_data_t)];
        ds1338_data_t _rtc_data{0};
        real_time_t _real_time;

        void convert_coordinate(const  ds1338_data_t *rtc_data, float *out_lat, float *out_lon, float *tz);

        void unix_to_time(const uint32_t unix_time, real_time_t *timeptr);
        uint32_t time_to_unix(const real_time_t *timeptr);
        uint8_t unix_to_weekday(const uint32_t unix_time);
        uint8_t is_leap_year(int16_t year);

        bool _rtc_working{true};
};









#endif//EDS1338_H_INCLUDED