#include "eDS1338.hpp"



// Конвертация: обычное число (HEX) -> двоично-десятичное (BCD)
static inline uint8_t rtc_dec_to_bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

// Конвертация: двоично-десятичное (BCD) -> обычное число (HEX)
static inline uint8_t rtc_bcd_to_dec(uint8_t val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

eDS1338::eDS1338(I2C_HandleTypeDef *i2c_obj, uint16_t address)
{
    _i2c_bus = i2c_obj;
    _dev_address = address;

    memset(_rtc_buffer, 0, sizeof(ds1338_data_t));
}


HAL_StatusTypeDef eDS1338::periodic(void)
{
    HAL_StatusTypeDef status;
    uint8_t reg_addr = 0x00;
    ds1338_data_t *ptr_rtc_buffer = (ds1338_data_t*)_rtc_buffer;
    
    // 1. Устанавливаем указатель адреса в DS1338 на регистр 0x00
    status = HAL_I2C_Master_Transmit(_i2c_bus, _dev_address, &reg_addr, 1, 100);
    if (status != HAL_OK) return status;
    
    // 2. Считываем сырые байты напрямую в целевую структуру
    status = HAL_I2C_Master_Receive(_i2c_bus, _dev_address, _rtc_buffer, sizeof(ds1338_data_t), 100);
    if (status != HAL_OK) return status;
    
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

    
    // Переменные float (latitude, longitude, time_zone) уже на своих местах и готовы к работе
    return HAL_OK;
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


// Преобразование координат
// Функция преобразования
/*
void eDS1338::convert_coordinate(const ds1338_data_t *rtc_data, float *out_lat, float *out_lon, float *tz) 
{
    if (rtc_data == NULL)    { return; }
    if (out_lat != NULL) { *out_lat = dms_to_float(rtc_data->lat_deg, rtc_data->lat_min, rtc_data->lat_sec); }
    if (out_lon != NULL) { *out_lon = dms_to_float(rtc_data->lon_deg, rtc_data->lon_min, rtc_data->lon_sec); }
    if (tz != NULL)      { *tz = (float)rtc_data->time_zone; }
}
*/
//->>>>>>>>>>>>


