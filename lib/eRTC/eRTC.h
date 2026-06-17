#ifndef EI2C_H_INCLUDED
#define EI2C_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <time_common.h>
#include <twilight.hpp>

#include "eLed.h"


#define STAT_SYMBOL     false
#define BLINK_SYMBOL    true


enum _SCREEN_CONF_
{
    CONF_RTC,
    CONF_LOC,
    CONF_CH
};

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


typedef struct 
{
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
    uint8_t control;
   
} rtc_data_t;

typedef struct 
{
    int16_t  lat_deg;         /**< широта, градусы  */
    uint8_t  lat_min;         /**< широта, минуты   */
    uint8_t  lat_sec;         /**< широта, секунды  */
    int16_t  lon_deg;         /**< долгота, градусы */  
    uint8_t  lon_min;         /**< долгота, минуты  */
    uint8_t  lon_sec;         /**< долгота, секунды */
    int8_t   time_zone;       /**< временная зона   */    
    
}__attribute__((__packed__)) loc_data_t;


/********************************************************************************************* */
class eRTC
{
    public:
        eRTC(I2C_HandleTypeDef *i2c_obj, uint16_t address);

        void init(void);
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
        I2C_HandleTypeDef *i2c_bus;
        uint16_t dev_address;

        bool is_config;
        real_time_t rtc_time = {0};
        loc_data_t rtc_location = {0};
        uint8_t rtc_buffer[sizeof(rtc_data_t)+sizeof(loc_data_t)];  
        uint8_t loc_buffer[sizeof(loc_data_t)];
        
        void convert_coordinate(const loc_data_t *data, int32_t *out_lat, int32_t *out_lon, int16_t *tz);
        void unix_to_time(const uint32_t unix_time, real_time_t *timeptr);
        uint32_t time_to_unix(const real_time_t *timeptr);
        uint8_t unix_to_weekday(const uint32_t unix_time);
        uint8_t is_leap_year(int16_t year);

        template <typename T, typename OP> 
        void change_operation(T *ptr_param, OP op, int16_t limit_min, int16_t limit_max);

};









#endif//EI2C_H_INCLUDED