#include "eDS1338.hpp"

template <typename T, typename OP> 
void change_operation(T &ptr_param, OP op, int16_t limit_min, int16_t limit_max);

// Конвертация: обычное число (HEX) -> двоично-десятичное (BCD)
static inline uint8_t rtc_dec_to_bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

// Конвертация: двоично-десятичное (BCD) -> обычное число (HEX)
static inline uint8_t rtc_bcd_to_dec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}
static inline float dms_to_float(int16_t deg, uint8_t min, uint8_t sec);
static inline void float_to_dms(float decimal, int16_t *deg, uint8_t *min, uint8_t *sec);

void eDS1338::rtc_suspend(void){ _rtc_working = false; }
void eDS1338::rtc_resume(void){ _rtc_working = true; }

/*******************************************************************************
 * Конструктор
 * i2c_obj - шина I2C
 * address - адрес часов на шине
 */
eDS1338::eDS1338(I2C_HandleTypeDef *i2c_obj, uint16_t address)
{
    _i2c_bus = i2c_obj;
    _dev_address = address;

    memset(_rtc_buffer, 0, sizeof(ds1338_data_t));
}

/*******************************************************************************
 * Вызывать периодически в главном цикле
 * Получение актуального времени и координат из RTC
 */
HAL_StatusTypeDef eDS1338::periodic(void)
{
    uint8_t start_addr = 0;
    ds1338_data_t *ptr_rtc_buffer = (ds1338_data_t*)_rtc_buffer;

    if(!_rtc_working) return HAL_OK;
    
    while(HAL_I2C_Master_Transmit(_i2c_bus, (_dev_address << 1), &start_addr, 1, HAL_I2C_ERROR_TIMEOUT ) != HAL_OK) 
    {
        return HAL_TIMEOUT;
    }

    while(HAL_I2C_Master_Receive(_i2c_bus, (_dev_address << 1), _rtc_buffer, sizeof(ds1338_data_t), HAL_I2C_ERROR_TIMEOUT) != HAL_OK) 
    {
        return HAL_TIMEOUT;
    }
    
    // 3. Конвертируем полученное BCD-время в нормальный вид (HEX/DEC)
    _real_time.second = rtc_bcd_to_dec(ptr_rtc_buffer->second & 0x7F); // Маска 0x7F убирает флаг CH (Clock Halt)
    _real_time.sec_comma = ptr_rtc_buffer->second & 1;
    _real_time.minute = rtc_bcd_to_dec(ptr_rtc_buffer->minute);
    _real_time.hour   = rtc_bcd_to_dec(ptr_rtc_buffer->hour & 0x3F);   // Маска для 24-часового формата
    _real_time.date   = rtc_bcd_to_dec(ptr_rtc_buffer->date);
    _real_time.month  = rtc_bcd_to_dec(ptr_rtc_buffer->month);
    _real_time.year   = rtc_bcd_to_dec(ptr_rtc_buffer->year) + 2000;
    _real_time.unix_time = time_to_unix(&_real_time);

    _real_time.day = unix_to_weekday(_real_time.unix_time);

    _real_time.latitude = ptr_rtc_buffer->latitude;
    _real_time.longitude = ptr_rtc_buffer->longitude;
    _real_time.time_zone = ptr_rtc_buffer->time_zone;

    _real_time.sec_only_day = ((uint32_t)(_real_time.hour) * 3600) + ((uint32_t)(_real_time.minute) * 60) + ((uint32_t)(_real_time.second));
    _real_time.sec_week = ((uint32_t)(_real_time.day) * 86400) + ((uint32_t)(_real_time.hour) * 3600) + ((uint32_t)(_real_time.minute) * 60) + ((uint32_t)(_real_time.second));

    
    return HAL_OK;
}


/*******************************************************************************
 * Настройка параметров RTC 
 */
HAL_StatusTypeDef eDS1338::change_parameter(Parameter parameter_name, TypeOp op)
{
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
    
    } ds1338_data_time_t;
    #pragma pack(pop)

    static bool start_change = false;

    if(op == TypeOp::APPLY_RTC || op == TypeOp::APPLY_LATITUDE || op == TypeOp::APPLY_LONGITUDE || op == TypeOp::APPLY_TIMEZONE)
    {
        uint16_t block_size = 0;
        uint16_t start_addr = 0;

        if(op == TypeOp::APPLY_RTC)
        {
            start_addr = 0;
            block_size = sizeof(ds1338_data_time_t);
            ds1338_data_time_t *ptr_rtc_buffer = (ds1338_data_time_t*)_rtc_buffer;

            ptr_rtc_buffer->second = 0; // запуск часов
            ptr_rtc_buffer->minute = rtc_dec_to_bcd(_real_time.minute);
            ptr_rtc_buffer->hour = rtc_dec_to_bcd(_real_time.hour);
            ptr_rtc_buffer->date = rtc_dec_to_bcd(_real_time.date);
            ptr_rtc_buffer->month = rtc_dec_to_bcd(_real_time.month);
            ptr_rtc_buffer->year = rtc_dec_to_bcd(_real_time.year - 2000);

        }

        else if(op == TypeOp::APPLY_LATITUDE)
        {
            start_addr = sizeof(ds1338_data_time_t);
            block_size = sizeof(float);
            float *ptr_latitude = (float*)_rtc_buffer;

            *ptr_latitude = _real_time.latitude;
        }

        else if(op == TypeOp::APPLY_LONGITUDE)
        {
            start_addr = sizeof(ds1338_data_time_t) + sizeof(float);
            block_size = sizeof(float);
            float *ptr_longitude = (float*)_rtc_buffer;
            *ptr_longitude = _real_time.longitude;
        }

        else if(op == TypeOp::APPLY_TIMEZONE)
        {
            start_addr = sizeof(ds1338_data_time_t) + sizeof(float) + sizeof(float);
            block_size = sizeof(float);
            float *ptr_timezone = (float*)_rtc_buffer;
            *ptr_timezone = _real_time.time_zone;
        }

        while(HAL_I2C_Mem_Write(_i2c_bus, (_dev_address << 1), start_addr, I2C_MEMADD_SIZE_8BIT, _rtc_buffer, block_size, HAL_I2C_ERROR_TIMEOUT ) != HAL_OK) 
        {
            return HAL_TIMEOUT;
        }
        
        start_change = false;
        return HAL_OK;
    }

/*----------------------------------------------------------------------------------------------------------------------------*/
    switch(parameter_name)
    {
        case Parameter::HOUR:   change_operation(_real_time.hour, op, 0, 23); break;
        case Parameter::MINUTE: change_operation(_real_time.minute, op, 0, 59); break;  
        case Parameter::DAY:    change_operation(_real_time.day, op, MONDAY, SUNDAY); break;   
        case Parameter::MONTH:  change_operation(_real_time.month, op, JANUARY, DECEMBER); break; 
        case Parameter::YEAR:   change_operation(_real_time.year, op, 2000, 2100); break;    

        case Parameter::DATE:
        {
            uint16_t limit_date_max;

            if(_real_time.month == APRIL || _real_time.month == JUNE || _real_time.month == SEPTEMBER || _real_time.month == NOVEMBER) 
            {
                limit_date_max = 30;
            }
            else if(_real_time.month == FEBRUARY)
            {
                if(is_leap_year(_real_time.year)) limit_date_max = 29;
                else limit_date_max = 28;
            }
            else limit_date_max = 31;

            change_operation(_real_time.date, op, 0, limit_date_max);
            break;
        } 

            
        /*
        case Parameter::LON_DEG:
        {
            int16_t temp_lon = temp_parameter.lon_deg; 
            change_operation(temp_lon, op, -180, 180);
            rtc_location.lon_deg = temp_lon;
        }; break; 

        case Parameter::LON_MIN:
        {
            change_operation(rtc_location.lon_min, op, 0, 60);
        }; break;

        case Parameter::LON_SEC:
        {
            change_operation(rtc_location.lon_sec, op, 0, 60);
        }; break;


            case Parameter::LAT_DEG:
            {
                int16_t temp_lat = rtc_location.lat_deg;
                change_operation(temp_lat, op, -90, 90); 
                rtc_location.lat_deg = temp_lat;
            }; break; 

            case Parameter::LAT_MIN:
            {
                change_operation(rtc_location.lat_min, op, 0, 60);
            }; break;

            case Parameter::LAT_SEC:
            {
                change_operation(rtc_location.lat_sec, op, 0, 60);
            }; break;

            case Parameter::TIMEZONE:
            {
                change_operation(rtc_location.time_zone, op, -12, +14);
            }; break;

        */
            default: ;break;
    }

    return HAL_OK;

}


uint32_t eDS1338::get_unix_time(void) { return _real_time.unix_time; }
uint8_t eDS1338::get_day(void) { return _real_time.day; }
uint8_t eDS1338::get_date(void) { return _real_time.date; }
uint8_t eDS1338::get_month(void) { return _real_time.month; }
uint16_t eDS1338::get_year(void) { return _real_time.year; }
uint8_t eDS1338::get_hour(void) { return _real_time.hour; }
uint8_t eDS1338::get_minute(void) { return _real_time.minute; }
bool eDS1338::get_sec_comma(void) { return (bool)(_real_time.sec_comma); }

uint32_t eDS1338::get_sec_only_day(void) { return _real_time.sec_only_day; }
uint32_t eDS1338::get_sec_week(void) { return _real_time.sec_week; }

int16_t eDS1338::get_longitude_degree(void)
{
    int16_t degree = 0;
    float_to_dms(_real_time.longitude, &degree, nullptr, nullptr);
    return degree;
}

int16_t eDS1338::get_longitude_min(void)
{
    uint8_t minute = 0;
    float_to_dms(_real_time.longitude, nullptr, &minute, nullptr);
    return minute;
}

int16_t eDS1338::get_longitude_sec(void)
{
    uint8_t second = 0;
    float_to_dms(_real_time.longitude, nullptr, &second, nullptr);
    return second;
}
//////////////////
int16_t eDS1338::get_latitude_degree(void)
{
    int16_t degree = 0;
    float_to_dms(_real_time.latitude, &degree, nullptr, nullptr);
    return degree;
}

int16_t eDS1338::get_latitude_min(void)
{
    uint8_t minute = 0;
    float_to_dms(_real_time.latitude, nullptr, &minute, nullptr);
    return minute;
}

int16_t eDS1338::get_latitude_sec(void)
{
    uint8_t second = 0;
    float_to_dms(_real_time.latitude, nullptr, nullptr, &second);
    return second;
}

int16_t eDS1338::get_timezone(void){ 

    return (uint16_t)_real_time.time_zone;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void eDS1338::get_civil_dawn(uint8_t &hour, uint8_t &minute) // Начало утренних гражданских сумерек
{
    real_time_t time_civil;
    unix_to_time(_real_time.twilight_rise, &time_civil);
    hour = time_civil.hour;
    minute = time_civil.minute;
}

void eDS1338::get_civil_dusk(uint8_t &hour, uint8_t &minute) // Конец вечерних гражданских сумерек
{
    real_time_t time_civil;
    unix_to_time(_real_time.twilight_set, &time_civil);
    hour = time_civil.hour;
    minute = time_civil.minute;
} 


/********************************************************************************************************* */
static inline float dms_to_float(int16_t deg, uint8_t min, uint8_t sec)
{
    float decimal = (float)deg;
    float fractional = ((float)min / 60.0f) + ((float)sec / 3600.0f);
    // Если градусы отрицательные, дробную часть нужно вычесть
    if (deg < 0) { return decimal - fractional; }
    return decimal + fractional;
}

static inline void float_to_dms(float decimal, int16_t *deg, uint8_t *min, uint8_t *sec)
{
    // 1. Выделяем знак (проверяем, отрицательные ли градусы)
    int sign = (decimal < 0.0f) ? -1 : 1;
    
    // Работаем с абсолютным (положительным) значением для удобства расчета
    if (decimal < 0.0f) { decimal = -decimal; }
    
    // 2. Получаем целую часть градусов
    int16_t d = (int16_t)decimal;
    
    // Находим остаток после градусов и переводим в минуты
    float fractional_min = (decimal - (float)d) * 60.0f;
    
    // 3. Получаем целую часть минут
    uint8_t m = (uint8_t)fractional_min;
    
    // Находим остаток после минут и переводим в секунды (с округлением)
    float fractional_sec = (fractional_min - (float)m) * 60.0f;
    
    // 4. Округляем секунды до ближайшего целого
    uint8_t s = (uint8_t)(fractional_sec + 0.5f);
    
    // Защита от переполнения при округлении секунды (например, если вышло 60 секунд)
    if (s >= 60) {
        s = 0;
        m++;
        if (m >= 60) {
            m = 0;
            d++;
        }
    }
    
    // 5. Возвращаем результаты (возвращаем исходный знак градусам)
    *deg = d * sign;
    *min = m;
    *sec = s;
}

/******************************************************************************************************** */
/*************************************************************************/
const uint32_t SEC_A_DAY  = 86400;

void eDS1338::unix_to_time(const uint32_t unix_time, real_time_t *timeptr)
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


uint32_t eDS1338::time_to_unix(const real_time_t *timeptr)
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



uint8_t eDS1338::unix_to_weekday(const uint32_t unix_time)
{
    // Находим количество полных дней, прошедших с 1 января 1970 года
    uint32_t days_since_epoch = unix_time / 86400; // 86400 секунд в сутках
    
    // 1 января 1970 года — это ЧЕТВЕРГ.
    // Чтобы ПОНЕДЕЛЬНИК стал равен 0 (согласно вашему enum _WEEK_DAYS_),
    // нам нужно добавить сдвиг +3 к общему количеству дней.
    return (uint8_t)((days_since_epoch + 3) % 7);	
}


uint8_t eDS1338::is_leap_year(int16_t year)
{
    // Год високосный, если он делится на 4 и при этом:
    // либо не делится на 100, либо делится на 400.
    return ((year & 3) == 0 && (year % 100 != 0 || year % 400 == 0));
}


/*** служебные функции ***********************************************************************************/
template <typename T, typename OP> 
void change_operation(T &ptr_param, OP op, int16_t limit_min, int16_t limit_max)
{
    using namespace nDS1338;

    switch(op)
    {
        case TypeOp::PLUS:
        {
            if((ptr_param) < limit_max) ++(ptr_param);
            else (ptr_param) = limit_min;
        }; break;

        case TypeOp::MINUS:
        {
            if((ptr_param) > limit_min) --(ptr_param);
            else (ptr_param) = limit_max;
        }; break;

        default: break;
    }
}


