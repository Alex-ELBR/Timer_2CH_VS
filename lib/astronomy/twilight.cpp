#include <stdint.h>

// ============================================================================
// ТАБЛИЦА ПОДСТАНОВОК (LUT) И СТРУКТУРЫ ДАННЫХ
// ============================================================================

// Массив синусов (1.0 = 10000) для углов от 0 до 90 градусов
static const int16_t sin_lut[91] = {
    0, 175, 349, 523, 698, 872, 1045, 1219, 1392, 1564, 1736, 1908, 2079, 2250, 2419, 2588, 2756, 2924, 3090, 3256,
    3420, 3584, 3746, 3907, 4067, 4226, 4384, 4540, 4695, 4848, 5000, 5150, 5299, 5446, 5592, 5736, 5878, 6018, 6157, 6293,
    6428, 6561, 6691, 6820, 6947, 7071, 7193, 7314, 7431, 7547, 7660, 7771, 7880, 7986, 8090, 8192, 8290, 8387, 8480, 8572,
    8660, 8746, 8829, 8910, 8988, 9063, 9135, 9205, 9272, 9336, 9397, 9455, 9511, 9563, 9613, 9659, 9703, 9744, 9781, 9816,
    9848, 9877, 9903, 9925, 9945, 9962, 9976, 9986, 9994, 9998, 10000
};

enum class TwilightType { Official, Civil, Nautical, Astronomical };

struct TwilightResult 
{
    uint32_t start_time; // Unix Time начала (восход/рассвет)
    uint32_t end_time;   // Unix Time конца (закат/сумерки)
    bool is_valid;       // false, если полярный день или ночь
};

// ============================================================================
// БЛОК МАТЕМАТИКИ И ТРИГОНОМЕТРИИ (БЕЗ RECURSION, БЕЗ FLOAT)
// ============================================================================

// 1. Целочисленный квадратный корень для int64_t (алгоритм побитового сдвига Бабша)
static int32_t fp_sqrt(int64_t v) 
{
    if (v <= 0) return 0;
    int64_t res = 0;
    int64_t bit = 1LL << 62; // Начинаем с наивысшего возможного бита для int64_t
    while (bit > v) bit >>= 2;
    while (bit != 0) {
        if (v >= res + bit) {
            v -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return (int32_t)res;
}

// 2. Итеративный Синус (Вход: сотые доли градуса [0, 36000), Выход: масштаб 10000)
static int32_t fp_sin(int32_t angle_scaled) 
{
    if (angle_scaled < 0) {
        angle_scaled = 36000 - (-angle_scaled % 36000);
    }
    if (angle_scaled >= 36000) {
        angle_scaled %= 36000;
    }
    
    int32_t sign = 1;
    if (angle_scaled >= 18000) {
        sign = -1;
        angle_scaled -= 18000;
    }
    if (angle_scaled > 9000) {
        angle_scaled = 18000 - angle_scaled;
    }
    
    int32_t degree = angle_scaled / 100;
    int32_t rem = angle_scaled % 100;
    
    if (degree >= 90) return sign * 10000;
    
    int32_t result = sin_lut[degree] + ((sin_lut[degree + 1] - sin_lut[degree]) * rem) / 100;
    return sign * result;
}

// 3. Косинус через сдвиг фазы синуса
static int32_t fp_cos(int32_t angle_scaled) 
{
    return fp_sin(angle_scaled + 9000);
}

// 4. Арккосинус через бинарный поиск по таблице (Вход: масштаб 10000, Выход: сотые доли градуса)
static int32_t fp_acos(int32_t val_fp) 
{
    if (val_fp > 10000)  val_fp = 10000;
    if (val_fp < -10000) val_fp = -10000;

    bool negative = false;
    if (val_fp < 0) {
        negative = true;
        val_fp = -val_fp;
    }

    int32_t low = 0;
    int32_t high = 90;
    int32_t idx = 0;
    int32_t target = 10000 - val_fp; // Используем свойство cos(x) = sin(90 - x)

    while (low <= high) {
        int32_t mid = (low + high) / 2;
        if (sin_lut[mid] <= target) {
            idx = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    int32_t angle_deg_scaled = idx * 100;
    if (idx < 90) {
        int32_t diff_lut = sin_lut[idx + 1] - sin_lut[idx];
        if (diff_lut > 0) {
            int32_t rem_val = target - sin_lut[idx];
            angle_deg_scaled += (rem_val * 100) / diff_lut;
        }
    }

    return negative ? (18000 - angle_deg_scaled) : angle_deg_scaled;
}

// 5. Арктангенс 2 (Вход: Y, X в масштабе 10000, Выход: сотые доли градуса [-18000, 18000])
static int32_t fp_atan2(int32_t y, int32_t x) 
{
    if (x == 0 && y == 0) return 0;

    int32_t abs_y = y < 0 ? -y : y;
    int32_t abs_x = x < 0 ? -x : x;
    int32_t angle;

    if (abs_x >= abs_y) {
        int32_t num = (int64_t)abs_y * 4500 / abs_x; 
        angle = num; 
    } else {
        int32_t num = (int64_t)abs_x * 4500 / abs_y;
        angle = 9000 - num;
    }

    if (x < 0) {
        if (y >= 0) angle = 18000 - angle;
        else        angle = angle - 18000;
    } else {
        if (y < 0)  angle = -angle;
    }

    return angle;
}

// ============================================================================
// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВРЕМЕНИ
// ============================================================================

// Высокоточный расчет порядкового номера дня в году без тяжелых библиотек по UTC
static int get_day_of_year(uint32_t unix_time) 
{
    uint32_t days = unix_time / 86400;
    int32_t epoch_days = days + 719468;
    int32_t era = (epoch_days >= 0 ? epoch_days : epoch_days - 146096) / 146097;
    uint32_t doe = static_cast<uint32_t>(epoch_days - era * 146097);
    uint32_t yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;
    uint32_t y = yoe + era * 400;
    uint32_t doy = doe - (365*yoe + yoe/4 - yoe/100);
    uint32_t mp = (5*doy + 2)/153;
    uint32_t d = doy - (153*mp+2)/5 + 1;
    uint32_t m = mp + (mp < 10 ? 3 : -9);
    y += (m <= 2);
    
    bool is_leap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
    static const int16_t days_before_month[13] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    
    int day_of_year = days_before_month[m] + d;
    if (is_leap && m > 2) {
        day_of_year++;
    }
    return day_of_year;
}

// ============================================================================
// ОСНОВНОЙ АЛГОРИТМ РАСЧЕТА СУМЕРЕК
// ============================================================================

TwilightResult calculate_twilight(uint32_t unix_time, int32_t lat_scaled, int32_t lon_scaled, int16_t tz_offset_scaled, TwilightType type) 
{
    TwilightResult result = {0, 0, false};
    
    int N = get_day_of_year(unix_time);
    
    // Долгота в часовой мере (масштаб 10000). 15 градусов = 1 час.
    int32_t lng_hour_fp = (lon_scaled * 100) / 15; 

    // Установка угла зенита с учетом атмосферной рефракции
    int32_t zenith_scaled = 9083; 
    switch (type) {
        case TwilightType::Civil:        zenith_scaled = 9683;  break; 
        case TwilightType::Nautical:     zenith_scaled = 10283; break; 
        case TwilightType::Astronomical: zenith_scaled = 10883; break; 
        default:                         zenith_scaled = 9083;  break;
    }

    auto calculate_half_day = [&](bool is_sunrise, uint32_t& out_time) -> bool {
        
        int32_t base_hour = is_sunrise ? 60000 : 180000;
        int32_t t_fp = N * 10000 + (base_hour - lng_hour_fp) / 24;

        // Средняя аномалия Солнца (M)
        int32_t M_scaled = (9856 * t_fp) / 1000000 - 328; 
        
        // Истинная долгота Солнца (L)
        int32_t sinM = fp_sin(M_scaled);
        int32_t sin2M = fp_sin(M_scaled * 2);
        int32_t L_scaled = M_scaled + (1916 * sinM) / 10000 + (20 * sin2M) / 10000 + 28263;
        L_scaled %= 36000;
        if (L_scaled < 0) L_scaled += 36000;

        // Прямое восхождение Солнца (RA)
        int32_t sinL = fp_sin(L_scaled);
        int32_t cosL = fp_cos(L_scaled);
        if (cosL == 0) cosL = 1; 
        
        int32_t tanL = (sinL * 10000) / cosL;
        int32_t y_ra = (9175 * tanL) / 10000;
        int32_t RA_scaled = fp_atan2(y_ra, 10000);
        if (RA_scaled < 0) RA_scaled += 36000;
        
        int32_t L_quad = (L_scaled / 9000) * 9000;
        int32_t RA_quad = (RA_scaled / 9000) * 9000;
        RA_scaled = RA_scaled + (L_quad - RA_quad);
        int32_t RA_hour_fp = (RA_scaled * 100) / 15; 

        // Склонение Солнца (Dec)
        int32_t sinDec = (3978 * fp_sin(L_scaled)) / 10000;
        
        int64_t cosDec_sq = 100000000LL - ((int64_t)sinDec * sinDec);
        int32_t cosDec = fp_sqrt(cosDec_sq);
        if (cosDec == 0) cosDec = 1; 

        // Вычисление локального часового угла Солнца (cosH)
        int64_t num = (int64_t)fp_cos(zenith_scaled) * 10000 - (int64_t)sinDec * fp_sin(lat_scaled);
        int64_t den = (int64_t)cosDec * fp_cos(lat_scaled);
        if (den == 0) return false;
        
        int32_t cosH_fp = (num * 10000) / den; 

        if (cosH_fp > 10000 || cosH_fp < -10000) return false;

        int32_t H_scaled = fp_acos(cosH_fp);
        if (is_sunrise) H_scaled = 36000 - H_scaled; 
        int32_t H_hour_fp = (H_scaled * 100) / 15;   

        // Расчет всемирного времени события (UT) в часах
        int32_t T_fp = H_hour_fp + RA_hour_fp - (657 * t_fp) / 100000 - 66220;
        int32_t UT_fp = T_fp - lng_hour_fp; 
        
        UT_fp %= 240000;
        if (UT_fp < 0) UT_fp += 240000;

        uint32_t seconds_of_day = ((uint64_t)UT_fp * 3600) / 10000;
        uint32_t start_of_day = (unix_time / 86400) * 86400;
        
        // Unix Time не зависит от часового пояса и хранится строго по UTC. Смещение удалено.
        out_time = start_of_day + seconds_of_day;
        return true;
    };

    bool sunrise_ok = calculate_half_day(true, result.start_time);
    bool sunset_ok  = calculate_half_day(false, result.end_time);

    if (sunrise_ok && sunset_ok) {
        result.is_valid = true;
    }
    return result;
}
