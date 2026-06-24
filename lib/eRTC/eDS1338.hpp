#ifndef EDS1338_H_INCLUDED
#define EDS1338_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <time_common.h>
#include <twilight.hpp>



// типы для параметра "op" функции change_parameter() 
enum _TYPE_OP_ 
{
    APPLY_RTC,
    APPLY_LOC_LON,
    APPLY_LOC_LAT,
    APPLY_LOC_TIMEZONE
};

// типы для параметра "parameter_name" функции change_parameter()
enum _TYPE_PARAMETER_NAME_RTC_
{
    CHANGE_HOUR,
    CHANGE_MINUTE,
    CHANGE_DAY,
    CHANGE_DATE,
    CHANGE_MONTH,
    CHANGE_YEAR,

    CHANGE_LON_DEG,
    CHANGE_LON_MIN,
    CHANGE_LON_SEC,
    CHANGE_LAT_DEG,
    CHANGE_LAT_MIN,
    CHANGE_LAT_SEC,
    CHANGE_TIMEZONE
};


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

typedef struct
{
    uint8_t  second;             /**< seconds after the minute - [ 0 to 59 ] */
    uint8_t  sec_comma;          /**< секундная точка */
    uint8_t  minute;             /**< minutes after the hour - [ 0 to 59 ] */
    uint8_t  hour;               /**< hours since midnight - [ 0 to 23 ] */
    uint8_t  day;                /**< days since Sunday - [ 0 to 6 ] */
    uint8_t  date;               /**< day of the month - [ 1 to 31 ] */
    uint8_t  month;              /**< months since January - [ 0 to 11 ] */
    uint16_t year;               /**< years since 1900 */
    uint32_t unix_time;
    uint32_t sec_only_day;       /** кол-во секунд с начала суток (с 00:00) **/
    uint32_t sec_week;           /** кол-во секунд с начала недели (с 00:00 ПН) **/
    uint32_t sun_set;            /** время заката солнца **/
    uint32_t sun_rise;           /** время восхода солнца **/
    uint32_t twilight_set;       /** время начала гражданских сумерек **/
    uint32_t twilight_rise;      /** время окончания гражданских сумерек **/
    bool     polar_day;          /** это полярный день? **/
    bool     polar_night;        /** это полярная ночь? **/
    float    latitude;           /**< географическая широта           */
    float    longitude;          /**< географическая долгота          */
    float    time_zone;          /**< часовой пояс                    */


} real_time_t; //структура данных времени


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
        void start_change(void);
        void stop_change(void);
        HAL_StatusTypeDef change_parameter(uint8_t parameter_name, uint16_t op);


    private:
        I2C_HandleTypeDef *_i2c_bus;
        uint16_t _dev_address;
        uint8_t _rtc_buffer[sizeof(ds1338_data_t)];
        ds1338_data_t _rtc_data{0};
        real_time_t _real_time;

        void convert_coordinate(const  ds1338_data_t *rtc_data, float *out_lat, float *out_lon, float *tz);

        template <typename T, typename OP> 
        void change_operation(T &ptr_param, OP op, int16_t limit_min, int16_t limit_max);

        void unix_to_time(const uint32_t unix_time, real_time_t *timeptr);
        uint32_t time_to_unix(const real_time_t *timeptr);
        uint8_t unix_to_weekday(const uint32_t unix_time);
        uint8_t is_leap_year(int16_t year);





};









#endif//EDS1338_H_INCLUDED