#include <eRTC.h>

#include "stm32f1xx_hal.h"
#include <config_device.h>
extern ELed led_1, led_2, led_3;


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

    int16_t  lon_deg;         /**< долгота, градусы */
    uint8_t  lon_min;         /**< долгота, минуты  */
    uint8_t  lon_sec;         /**< долгота, секунды */
    int16_t  lat_deg;         /**< широта, градусы  */
    uint8_t  lat_min;         /**< широта, минуты   */
    uint8_t  lat_sec;         /**< широта, секунды  */
    int8_t   time_zone;       /**< временная зона   */    
    
}__attribute__((__packed__)) ptr_rtc_buffer_t;

typedef struct 
{
    int16_t  lon_deg;         /**< долгота, градусы */
    uint8_t  lon_min;         /**< долгота, минуты  */
    uint8_t  lon_sec;         /**< долгота, секунды */
    
}__attribute__((__packed__)) loc_lon_data_t;

typedef struct 
{
    int16_t  lat_deg;         /**< широта, градусы */
    uint8_t  lat_min;         /**< широта, минуты  */
    uint8_t  lat_sec;         /**< широта, секунды */
    
}__attribute__((__packed__)) loc_lat_data_t;

typedef struct 
{
    int8_t  time_zone;         /**< временная зона */
    
}__attribute__((__packed__)) loc_tz_data_t;

/// @brief ////////////////////////////////////////////////////////////////////////////////////////////////
constexpr uint16_t START_ADDRESS_RTC = 0;
constexpr uint16_t START_ADDRESS_LONGITUDE = (uint16_t)(START_ADDRESS_RTC + sizeof(rtc_data_t));
constexpr uint16_t START_ADDRESS_LATITUDE = (uint16_t)(START_ADDRESS_LONGITUDE + sizeof(loc_lon_data_t));
constexpr uint16_t START_ADDRESS_TIMEZONE = (uint16_t)(START_ADDRESS_LATITUDE + sizeof(loc_lat_data_t));


/// @brief /////////////////////////////////////////////////////////////////////////////////////////////////
/// @param val 
/// @return 
uint8_t dec_to_bcd(uint8_t val) { return((val/10*16) + (val%10)); }
uint8_t bcd_to_dec(uint8_t val) { return((val/16*10) + (val%16)); }



/// @brief /////////////////////////////////////////////////////////////////////////////////////////////////
/// @param i2c_obj 
/// @param address 
/// --------------------------------------------------------------------------------------------------------
eRTC::eRTC(I2C_HandleTypeDef *i2c_obj, uint16_t address)
{
    i2c_bus = i2c_obj;
    dev_address = address;
    rtc_time = {0};
    is_config = false;
    memset(rtc_buffer, 0, sizeof(rtc_data_t));
    memset(loc_buffer, 0, sizeof(loc_data_t));
}

void eRTC::init()
{
/*
    __HAL_RCC_I2C1_CLK_ENABLE();

    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 400000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    GPIO_InitTypeDef GPIO_InitStruct; 
    GPIO_InitStruct.Pin = I2C_BUS_SCL_PIN|I2C_BUS_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_BUS_PORT, &GPIO_InitStruct);

    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        while(1);
    }
*/
}


HAL_StatusTypeDef eRTC::periodic(void)
{
    uint8_t start_addr = START_ADDRESS_RTC;
    ptr_rtc_buffer_t *ptr_rtc_buf = (ptr_rtc_buffer_t*)rtc_buffer;

    TwilightResult civil = {0};
    TwilightResult sun = {0};

    if(is_config) return HAL_OK; // если запущена настройка часов - выходим без чтения

    /////////////////////////////////////
    while(HAL_I2C_Master_Transmit(i2c_bus, (dev_address << 1), &start_addr, 1, HAL_I2C_ERROR_TIMEOUT ) != HAL_OK) 
    {
        return HAL_TIMEOUT;
    }

    while(HAL_I2C_Master_Receive(i2c_bus, (dev_address << 1), rtc_buffer, sizeof(ptr_rtc_buffer_t), HAL_I2C_ERROR_TIMEOUT) != HAL_OK) 
    {
        return HAL_TIMEOUT;
    }

    /////////////////////////////////////
    rtc_time.unix_time = time_to_unix(&rtc_time);
    rtc_time.sec_comma = rtc_time.second & 1;
    rtc_time.day = unix_to_weekday(rtc_time.unix_time);


    rtc_time.hour =  bcd_to_dec(ptr_rtc_buf->hour); 
    rtc_time.minute =  bcd_to_dec(ptr_rtc_buf->minute);
    rtc_time.second =  bcd_to_dec(ptr_rtc_buf->second); 
    rtc_time.date =  bcd_to_dec(ptr_rtc_buf->date); 
    rtc_time.month =  bcd_to_dec(ptr_rtc_buf->month); 
    rtc_time.year =  bcd_to_dec(ptr_rtc_buf->year) + 2000; 

    rtc_location.lon_deg = ptr_rtc_buf->lon_deg;
    rtc_location.lon_min = ptr_rtc_buf->lon_min;
    rtc_location.lon_sec = ptr_rtc_buf->lon_sec;    
    rtc_location.lat_deg = ptr_rtc_buf->lat_deg;
    rtc_location.lat_min = ptr_rtc_buf->lat_min;
    rtc_location.lat_sec = ptr_rtc_buf->lat_sec;
    rtc_location.time_zone = ptr_rtc_buf->time_zone;


    rtc_time.sec_only_day = ((uint32_t)(rtc_time.hour) * 3600) + ((uint32_t)(rtc_time.minute) * 60) + ((uint32_t)(rtc_time.second));
    rtc_time.sec_week = ((uint32_t)(rtc_time.day) * 86400) + ((uint32_t)(rtc_time.hour) * 3600) + ((uint32_t)(rtc_time.minute) * 60) + ((uint32_t)(rtc_time.second));
     

    int32_t lat = 0;
    int32_t lon = 0;
    int16_t tz = 0;  

    convert_coordinate(&rtc_location, &lat, &lon, &tz);
    civil = calculate_twilight(rtc_time.unix_time, lat, lon, tz, TwilightType::Civil);
    sun = calculate_twilight(rtc_time.unix_time, lat, lon, tz, TwilightType::Official);

    rtc_time.polar_day = civil.polar_day;
    rtc_time.polar_night = civil.polar_night;

    rtc_time.twilight_rise = civil.start_time;
    rtc_time.twilight_set = civil.end_time;

    rtc_time.sun_rise = sun.start_time;
    rtc_time.sun_set = sun.end_time;

    return HAL_OK;
}

uint32_t eRTC::get_unix_time(void) { return rtc_time.unix_time; }
uint8_t eRTC::get_day(void) { return rtc_time.day; }
uint8_t eRTC::get_date(void) { return rtc_time.date; }
uint8_t eRTC::get_month(void) { return rtc_time.month; }
uint16_t eRTC::get_year(void) { return rtc_time.year; }
uint8_t eRTC::get_hour(void) { return rtc_time.hour; }
uint8_t eRTC::get_minute(void) { return rtc_time.minute; }
bool eRTC::get_sec_comma(void) { return (bool)(rtc_time.sec_comma); }

uint32_t eRTC::get_sec_only_day(void) { return rtc_time.sec_only_day; }
uint32_t eRTC::get_sec_week(void) { return rtc_time.sec_week; }

int16_t eRTC::get_longitude_degree(void){ return rtc_location.lon_deg; }
int16_t eRTC::get_longitude_min(void){ return rtc_location.lon_min; }
int16_t eRTC::get_longitude_sec(void){ return rtc_location.lon_sec; }

int16_t eRTC::get_latitude_degree(void){ return rtc_location.lat_deg; }
int16_t eRTC::get_latitude_min(void){ return rtc_location.lat_min; }
int16_t eRTC::get_latitude_sec(void){ return rtc_location.lat_sec; }

int16_t eRTC::get_timezone(void){ return rtc_location.time_zone; }

///////////////////////////////////////////////////////////////////////////////////////////////////
void eRTC::get_civil_dawn(uint8_t &hour, uint8_t &minute) // Начало утренних гражданских сумерек
{
    real_time_t user_time;
    unix_to_time(rtc_time.twilight_rise, &user_time);
    hour = user_time.hour;
    minute = user_time.minute;

}

void eRTC::get_civil_dusk(uint8_t &hour, uint8_t &minute) // Конец вечерних гражданских сумерек
{
    real_time_t user_time;
    unix_to_time(rtc_time.twilight_set, &user_time);
    hour = user_time.hour;
    minute = user_time.minute;

}

/******************************************************************************************************************** */
void eRTC::start_change(void)
{
    is_config = true;
}

void eRTC::stop_change(void)
{
    is_config = false;
}



/// @brief //////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @param parameter_name - имя параметра
/// @param op             - тип операции (PLUS, MINUS или APPLY)
/// @return               - HAL_OK если все норм
HAL_StatusTypeDef eRTC::change_parameter(uint8_t parameter_name, uint16_t op)
{
    rtc_data_t *ptr_rtc_buf = (rtc_data_t*)rtc_buffer;
    

        switch(parameter_name)
        {
            case CHANGE_HOUR:
            {
                change_operation(rtc_time.hour, op, 0, 23);
            }; break;

            case CHANGE_MINUTE:
            {
                change_operation(rtc_time.minute, op, 0, 59);
            }; break;  

            case CHANGE_DAY:
            {
                change_operation(rtc_time.day, op, MONDAY, SUNDAY);
            }; break;   

            case CHANGE_DATE:
            {
                uint16_t limit_date_max;

                if((rtc_time.month == APRIL)||(rtc_time.month == JUNE)||(rtc_time.month == SEPTEMBER)||(rtc_time.month == NOVEMBER)) 
                {
                    limit_date_max = 30;
                }
                else if(rtc_time.month == FEBRUARY)
                {
                    if(is_leap_year(rtc_time.year)) limit_date_max = 29;
                    else limit_date_max = 28;
                }
                else limit_date_max = 31;

                change_operation(rtc_time.date, op, 0, limit_date_max);
            }; break;  

            case CHANGE_MONTH:
            {
                change_operation(rtc_time.month, op, JANUARY, DECEMBER);
            }; break; 

            case CHANGE_YEAR:
            {
                change_operation(rtc_time.year, op, 2000, 2100);
            }; break;    
            
            case CHANGE_LON_DEG:
            {
                int16_t temp_lon = rtc_location.lon_deg; 
                change_operation(temp_lon, op, -180, 180);
                rtc_location.lon_deg = temp_lon;
            }; break; 

            case CHANGE_LON_MIN:
            {
                change_operation(rtc_location.lon_min, op, 0, 60);
            }; break;

            case CHANGE_LON_SEC:
            {
                change_operation(rtc_location.lon_sec, op, 0, 60);
            }; break;


            case CHANGE_LAT_DEG:
            {
                int16_t temp_lat = rtc_location.lat_deg;
                change_operation(temp_lat, op, -90, 90); 
                rtc_location.lat_deg = temp_lat;
            }; break; 

            case CHANGE_LAT_MIN:
            {
                change_operation(rtc_location.lat_min, op, 0, 60);
            }; break;

            case CHANGE_LAT_SEC:
            {
                change_operation(rtc_location.lat_sec, op, 0, 60);
            }; break;

            case CHANGE_TIMEZONE:
            {
                change_operation(rtc_location.time_zone, op, -12, +14);
            }; break;


            default: ;break;
        }

    if(op == APPLY_RTC)
    {
        memset(rtc_buffer, 0, sizeof(rtc_data_t));

        //const uint16_t start_addr = START_ADDRESS_RTC;

        ptr_rtc_buf->second = 0; // запуск часов
        ptr_rtc_buf->minute = dec_to_bcd(rtc_time.minute);
        ptr_rtc_buf->hour = dec_to_bcd(rtc_time.hour);
        ptr_rtc_buf->date = dec_to_bcd(rtc_time.date);
        ptr_rtc_buf->month = dec_to_bcd(rtc_time.month);
        ptr_rtc_buf->year = dec_to_bcd(rtc_time.year - 2000);


        while(HAL_I2C_Mem_Write(i2c_bus, (dev_address << 1), START_ADDRESS_RTC, I2C_MEMADD_SIZE_8BIT, rtc_buffer, sizeof(rtc_data_t), HAL_I2C_ERROR_TIMEOUT ) != HAL_OK) 
        {
            return HAL_TIMEOUT;
        }

        stop_change();
    }

    if(op == APPLY_LOC_LON)
    {
        //const uint16_t start_addr = (uint16_t)(START_ADDRESS_RTC + sizeof(rtc_data_t));

        uint8_t loc_lon_buffer[sizeof(loc_lon_data_t)];
        loc_lon_data_t *ptr_loc_buf = (loc_lon_data_t*)loc_lon_buffer;
        ptr_loc_buf->lon_deg = rtc_location.lon_deg;
        ptr_loc_buf->lon_min = rtc_location.lon_min;
        ptr_loc_buf->lon_sec = rtc_location.lon_sec;

        while(HAL_I2C_Mem_Write(i2c_bus, (dev_address << 1), START_ADDRESS_LONGITUDE, I2C_MEMADD_SIZE_8BIT, loc_lon_buffer, sizeof(loc_lon_data_t), HAL_I2C_ERROR_TIMEOUT ) != HAL_OK) 
        {
            return HAL_TIMEOUT;
        }
    }  

    if(op == APPLY_LOC_LAT)
    {
        //const uint16_t start_addr = (uint16_t)(START_ADDRESS_RTC + sizeof(rtc_data_t) + sizeof(loc_lon_data_t));

        uint8_t loc_lat_buffer[sizeof(loc_lat_data_t)];
        loc_lat_data_t *ptr_loc_buf = (loc_lat_data_t*)loc_lat_buffer;
        ptr_loc_buf->lat_deg = rtc_location.lat_deg;
        ptr_loc_buf->lat_min = rtc_location.lat_min;
        ptr_loc_buf->lat_sec = rtc_location.lat_sec;

        while(HAL_I2C_Mem_Write(i2c_bus, (dev_address << 1), START_ADDRESS_LATITUDE, I2C_MEMADD_SIZE_8BIT, loc_lat_buffer, sizeof(loc_lat_data_t), HAL_I2C_ERROR_TIMEOUT ) != HAL_OK) 
        {
            return HAL_TIMEOUT;
        }

        stop_change();
    }

    if(op == APPLY_LOC_TIMEZONE)
    {
        //const uint16_t start_addr = (uint16_t)(START_ADDRESS_RTC + sizeof(rtc_data_t) + sizeof(loc_lon_data_t) + sizeof(loc_lat_data_t));

        uint8_t loc_tz_buffer[sizeof(loc_tz_data_t)];
        loc_tz_data_t *ptr_loc_buf = (loc_tz_data_t*)loc_tz_buffer;
        ptr_loc_buf->time_zone = rtc_location.time_zone;

        while(HAL_I2C_Mem_Write(i2c_bus, (dev_address << 1), START_ADDRESS_TIMEZONE, I2C_MEMADD_SIZE_8BIT, loc_tz_buffer, sizeof(loc_tz_data_t), HAL_I2C_ERROR_TIMEOUT ) != HAL_OK) 
        {
            return HAL_TIMEOUT;
        }

        stop_change();
    }
    return HAL_OK;
}

/*** служебные функции ***********************************************************************************/
template <typename T, typename OP> 
void eRTC::change_operation(T &ptr_param, OP op, int16_t limit_min, int16_t limit_max)
{
    switch(op)
    {
        case PLUS:
        {
            if((ptr_param) < limit_max) ++(ptr_param);
            else (ptr_param) = limit_min;
        }; break;

        case MINUS:
        {
            if((ptr_param) > limit_min) --(ptr_param);
            else (ptr_param) = limit_max;
        }; break;
    }
}

/********************************************************************************************************* */
// Преобразование координат
// Функция преобразования
void eRTC::convert_coordinate(const loc_data_t *data, int32_t *out_lat, int32_t *out_lon, int16_t *tz) 
{
    // Защита от передачи нулевых указателей (предотвращает HardFault)
    if (data == nullptr) return;

    // 1. Считаем модуль дробной части в сотых долях градуса.
    // Явно приводим все аргументы к int32_t, чтобы избежать предупреждений о знаковости (signed/unsigned)
    int32_t lat_frac = ((int32_t)data->lat_min * 6000 + (int32_t)data->lat_sec * 100 + 1800) / 3600;
    int32_t lon_frac = ((int32_t)data->lon_min * 6000 + (int32_t)data->lon_sec * 100 + 1800) / 3600;

    // 2. Объединяем градусы и дробную часть с учетом знака (для южного и западного полушарий)
    if (out_lat != nullptr) {
        if (data->lat_deg >= 0) {
            *out_lat = ((int32_t)data->lat_deg * 100) + lat_frac;
        } else {
            *out_lat = ((int32_t)data->lat_deg * 100) - lat_frac;
        }
    }

    if (out_lon != nullptr) {
        if (data->lon_deg >= 0) {
            *out_lon = ((int32_t)data->lon_deg * 100) + lon_frac;
        } else {
            *out_lon = ((int32_t)data->lon_deg * 100) - lon_frac;
        }
    }

    // 3. Конвертация часового пояса в формат ЧЧММ
    if (tz != nullptr) {
        *tz = (int16_t)((int32_t)data->time_zone * 100);
    }
}

/******************************************************************************************************** */
/*************************************************************************/
const uint32_t SEC_A_DAY  = 86400;

void eRTC::unix_to_time(const uint32_t unix_time, real_time_t *timeptr)
{
	uint32_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint32_t time;

	// Количество секунд, прошедших с начала текущих суток
	time = unix_time % SEC_A_DAY; // SEC_A_DAY должен быть равен 86400
	
	// Вычисление дня недели строго под ваш enum (MONDAY = 0 ... SUNDAY = 6)
	// 1 января 1970 — четверг. Сдвиг +3 делает понедельник нулевой точкой.
	uint32_t days_since_epoch = unix_time / 86400;
	timeptr->day = (days_since_epoch + 3) % 7; 

	// Математический алгоритм Флагерна для вычисления даты
	a = ((unix_time + 43200) / (86400 >> 1)) + (2440587 << 1) + 1;
	a >>= 1;
	a += 32044;
	b = (4 * a + 3) / 146097;
	a = a - (146097 * b) / 4;
	c = (4 * a + 3) / 1461;
	a = a - (1461 * c) / 4;
	d = (5 * a + 2) / 153;
	
	// Заполнение календаря человека
	timeptr->date = a - (153 * d + 2) / 5 + 1;
	timeptr->month = (d + 3 - 12 * (d / 10));
	timeptr->year = 100 * b + c - 4800 + (d / 10);
	
	// Заполнение времени суток
	timeptr->hour = (time / 3600);
	timeptr->minute = (time % 3600) / 60;
	timeptr->second = (time % 3600) % 60;
}


uint32_t eRTC::time_to_unix(const real_time_t *timeptr)
{
	int32_t a;
	int32_t y;
	int32_t m;
	uint32_t Uday;
	uint32_t unix_time;

	// ИСПРАВЛЕНИЕ 1: Перевод критических переменных в 32-битные знаковые типы,
	// чтобы избежать переполнения при умножении года на 365
	a = (14 - (int32_t)timeptr->month) / 12;
	y = (int32_t)timeptr->year + 4800 - a;
	m = (int32_t)timeptr->month + (12 * a) - 3;

	// Расчет количества дней с Юлианского периода до Эпохи Unix (2440588)
	// Все константы автоматически обрабатываются процессором как 32-битные
	Uday = (timeptr->date + ((153 * m + 2) / 5) + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045) - 2440588;
	
	// Перевод дней в секунды
	unix_time = Uday * SEC_A_DAY; // SEC_A_DAY = 86400
	
	// Добавление времени суток
	unix_time += (uint32_t)timeptr->second + 
	             ((uint32_t)timeptr->minute * 60) + 
	             ((uint32_t)timeptr->hour * 3600);

	return unix_time;
}



uint8_t eRTC::unix_to_weekday(const uint32_t unix_time)
{
    // Находим количество полных дней, прошедших с 1 января 1970 года
    uint32_t days_since_epoch = unix_time / 86400; // 86400 секунд в сутках
    
    // 1 января 1970 года — это ЧЕТВЕРГ.
    // Чтобы ПОНЕДЕЛЬНИК стал равен 0 (согласно вашему enum _WEEK_DAYS_),
    // нам нужно добавить сдвиг +3 к общему количеству дней.
    return (uint8_t)((days_since_epoch + 3) % 7);	
}


uint8_t eRTC::is_leap_year(int16_t year)
{
    // Год високосный, если он делится на 4 и при этом:
    // либо не делится на 100, либо делится на 400.
    return ((year & 3) == 0 && (year % 100 != 0 || year % 400 == 0));
}
