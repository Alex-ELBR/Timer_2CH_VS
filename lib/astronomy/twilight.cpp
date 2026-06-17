#include <stdint.h>
#include <math.h>

// Константа перевода градусов в радианы и обратно
#define DEG_TO_RAD (3.141592653589793f / 180.0f)
#define RAD_TO_DEG (180.0f / 3.141592653589793f)

enum class TwilightType { 
    Official, 
    Civil, 
    Nautical, 
    Astronomical 
};

struct TwilightResult 
{
    uint32_t start_time; // Unix Time начала (восход/рассвет)
    uint32_t end_time;   // Unix Time конца (закат/сумерки)
    bool is_valid;       // false, если полярный день или ночь
};

// Функция определения порядкового номера дня в году из Unix Time
static int get_day_of_year(uint32_t unix_time) {
    uint32_t days_since_epoch = unix_time / 86400;
    uint32_t rem_days = days_since_epoch % 1461;
    
    if (rem_days >= 365) {
        rem_days -= 365;
        if (rem_days >= 365) {
            rem_days -= 365;
            if (rem_days >= 366) { 
                rem_days -= 366;
            }
        }
    }
    return rem_days + 1;
}

TwilightResult calculate_twilight(uint32_t unix_time, int32_t lat_scaled, int32_t lon_scaled, int16_t tz_offset_scaled, TwilightType type) 
{
    TwilightResult result = {0, 0, false};
    
    int N = get_day_of_year(unix_time);
    
    // Переводим входные масштабированные координаты (5575 -> 55.75) в обычный float
    float lat = (float)lat_scaled / 100.0f;
    float lon = (float)lon_scaled / 100.0f;

    // Переводим часовой пояс (300 -> 3.0 часа -> в минуты)
    float tz_minutes = ((float)(tz_offset_scaled / 100) * 60.0f) + (float)(tz_offset_scaled % 100);

    // Угол зенита солнца в градусах
    float zenith = 90.8333f; 
    switch (type) {
        case TwilightType::Official:     zenith = 90.8333f; break; // 90° 50'
        case TwilightType::Civil:        zenith = 96.0f;    break; // 96°
        case TwilightType::Nautical:     zenith = 102.0f;   break; // 102°
        case TwilightType::Astronomical: zenith = 108.0f;   break; // 108°
    }

    // 1. Расчет солнечной геометрии (уравнение времени EoT и склонение Dec)
    float b_angle = (360.0f * (float)(N - 81)) / 365.0f;
    float b_rad = b_angle * DEG_TO_RAD;

    // Уравнение Времени в минутах
    float eot = 9.87f * sinf(2.0f * b_rad) - 7.53f * cosf(b_rad) - 1.5f * sinf(b_rad);

    // Склонение Солнца
    float dec = 23.45f * sinf(b_rad);

    // 2. Вычисление часового угла H
    float zenith_rad = zenith * DEG_TO_RAD;
    float lat_rad = lat * DEG_TO_RAD;
    float dec_rad = dec * DEG_TO_RAD;

    // cos(H) = (cos(zenith) - sin(lat)*sin(dec)) / (cos(lat)*cos(dec))
    float cosH = (cosf(zenith_rad) - sinf(lat_rad) * sinf(dec_rad)) / (cosf(lat_rad) * cosf(dec_rad));

    // Проверка на полярный день или полярную ночь
    if (cosH > 1.0f || cosH < -1.0f) {
        return result; // возвращает is_valid = false
    }

    // Часовой угол в градусах, затем переводим в минуты времени (1 градус = 4 минуты)
    float H_deg = acosf(cosH) * RAD_TO_DEG;
    float H_minutes = H_deg * 4.0f;

    // 3. Находим истинный полдень (Solar Noon) в минутах от начала суток
    // 720 минут = 12 часов. Вычитаем долготу (4 минуты на градус)
    float solar_noon_minutes = 720.0f - (lon * 4.0f) - eot + tz_minutes;

    // 4. Расчет финального времени в минутах от начала суток
    float sunrise_minutes = solar_noon_minutes - H_minutes;
    float sunset_minutes = solar_noon_minutes + H_minutes;

    // Защита от переходов через границы суток
    if (sunrise_minutes < 0.0f) sunrise_minutes += 1440.0f;
    if (sunset_minutes >= 1440.0f) sunset_minutes -= 1440.0f;

    // 5. Перевод минут в Unix Time (с точностью до секунд)
    uint32_t day_start_unix = (unix_time / 86400) * 86400;
    
    // Округляем минуты до целых секунд (умножаем на 60)
    result.start_time = day_start_unix + (uint32_t)roundf(sunrise_minutes * 60.0f);
    result.end_time = day_start_unix + (uint32_t)roundf(sunset_minutes * 60.0f);
    result.is_valid = true;

    return result;
}
