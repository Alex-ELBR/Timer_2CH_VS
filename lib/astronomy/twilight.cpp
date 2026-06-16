#include "twilight.hpp"

/**
 * ТАБЛИЦА СИНУСОВ (Look-Up Table)
 * Индекс массива — угол в целых градусах от 0 до 90.
 * Значения умножены на 10000 (масштаб 1:10000), чтобы исключить числа с плавающей точкой.
 * Пример: sin(30°) = 0.5 -> в таблице 5000. sin(90°) = 1.0 -> в таблице 10000.
 */
static const int16_t sin_lut[91] = {
    0, 175, 349, 523, 698, 872, 1045, 1219, 1392, 1564,
    1736, 1908, 2079, 2250, 2419, 2588, 2756, 2924, 3090, 3256,
    3420, 3584, 3746, 3907, 4067, 4226, 4384, 4540, 4695, 4848,
    5000, 5150, 5299, 5446, 5592, 5736, 5878, 6018, 6157, 6293,
    6428, 6561, 6691, 6820, 6947, 7071, 7193, 7314, 7431, 7547,
    7660, 7771, 7880, 7986, 8090, 8192, 8290, 8387, 8480, 8572,
    8660, 8746, 8829, 8910, 8988, 9063, 9135, 9205, 9272, 9336,
    9397, 9455, 9511, 9563, 9613, 9659, 9703, 9744, 9781, 9816,
    9848, 9877, 9903, 9925, 9945, 9962, 9976, 9986, 9994, 9998, 10000
};

/**
 * @brief Целочисленный синус угла.
 * @param angle_scaled Угол в сотых долях градуса (360° = 36000).
 * @return Значение синуса в масштабе 10000 (от -10000 до 10000).
 */
static int32_t fp_sin(int32_t angle_scaled) {
    // Приведение угла к диапазону [0, 36000)
    while (angle_scaled < 0) angle_scaled += 36000;
    angle_scaled %= 36000;
    
    int32_t degree = angle_scaled / 100; // Целые градусы
    int32_t rem = angle_scaled % 100;    // Остаток (сотые доли градуса)
    
    // Проекция угла на соответствующую четверть тригонометрического круга
    if (degree <= 90) {
        if (degree == 90) return sin_lut[90];
        // Линейная интерполяция между соседними элементами таблицы для точности
        return sin_lut[degree] + ((sin_lut[degree + 1] - sin_lut[degree]) * rem) / 100;
    } else if (degree <= 180) {
        return fp_sin((180 - degree) * 100 - rem); // 2-я четверть
    } else if (degree <= 270) {
        return -fp_sin((degree - 180) * 100 + rem); // 3-я четверть
    } else {
        return -fp_sin((360 - degree) * 100 - rem); // 4-я четверть
    }
}

/**
 * @brief Целочисленный косинус угла.
 * Использует сдвиг фазы синуса: cos(x) = sin(x + 90°).
 */
static int32_t fp_cos(int32_t angle_scaled) {
    return fp_sin(angle_scaled + 9000); // 90 градусов = 9000 сотых долей
}

/**
 * @brief Целочисленный арккосинус (acos).
 * @param val Входное значение в масштабе 10000 (от -10000 до 10000).
 * @return Угол в сотых долях градуса (от 0 до 18000).
 */
static int32_t fp_acos(int32_t val) {
    if (val >= 10000) return 0;
    if (val <= -10000) return 18000;
    
    // Бинарный поиск угла по таблице косинусов (быстрее разложения в ряд на Cortex-M3)
    int32_t low = 0, high = 18000, mid;
    while (high - low > 5) { // Точность схождения — 0.05 градуса
        mid = (low + high) / 2;
        if (fp_cos(mid) > val) low = mid; // Так как cos убывает на [0, 180]
        else high = mid;
    }
    return (low + high) / 2;
}

/**
 * @brief Функция atan2 (арктангенс y/x с учетом знаков четверти).
 * @return Угол в сотых долях градуса от 0 до 36000.
 */
static int32_t fp_atan2(int32_t y, int32_t x) {
    if (x == 0) return (y >= 0) ? 9000 : 27000;
    
    int32_t angle;
    int32_t abs_y = y < 0 ? -y : y;
    int32_t abs_x = x < 0 ? -x : x;
    
    // Линейная аппроксимация арктангенса (быстрая и не требует памяти)
    if (abs_x >= abs_y) {
        angle = (4500 * abs_y) / abs_x;
    } else {
        angle = 9000 - (4500 * abs_x) / abs_y;
    }
    // Коррекция угла в зависимости от квадранта геометрии x и y
    if (x < 0) angle = 18000 - angle;
    if (y < 0) angle = 36000 - angle;
    return angle;
}

/**
 * @brief Расчет порядкового номера дня в году (Day of Year: 1-366) на основе Unix Time.
 */
static int get_day_of_year(uint32_t unix_time) {
    uint32_t days = unix_time / 86400; // Всего дней с эпохи 1970 года
    int year = 1970;
    
    while (true) {
        // Проверка года на високосность
        bool is_leap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        int days_in_year = is_leap ? 366 : 365;
        if (days < (uint32_t)days_in_year) break;
        days -= days_in_year;
        year++;
    }
    return days + 1; // Возвращаем текущий день от 1 января (1-индексированный)
}

TwilightResult calculate_twilight(uint32_t unix_time, int32_t lat_scaled, int32_t lon_scaled, int16_t tz_offset_scaled, TwilightType type) {
    TwilightResult result = {0, 0, false};
    
    // 1. ПАРСИНГ ЧАСОВОГО ПОЯСА И ПЕРЕВОД В СЕКУНДЫ
    // Пример: 530 (это +05:30) -> tz_hours = 5, tz_minutes = 30. Суммарно 19800 секунд.
    int32_t tz_hours = tz_offset_scaled / 100;
    int32_t tz_minutes = tz_offset_scaled % 100;
    int32_t tz_offset_seconds = (tz_hours * 3600) + (tz_minutes * 60);

    // Порядковый номер дня (N) берется строго по базовому UTC
    int N = get_day_of_year(unix_time);
    
    // 2. ДОЛГОТА В ЧАСОВОЙ МЕРЕ
    // В астрономии 15 градусов долготы = 1 часу времени.
    // lng_hour_fp имеет масштаб 10000 (умножаем на 10000, делим на 15 * 100)
    int32_t lng_hour_fp = (lon_scaled * 10000) / 1500; 

    // 3. УСТАНОВКА УГЛА ЗЕНИТА С УЧЕТОМ АТМОСФЕРНОЙ РЕФРАКЦИИ
    // Рефракция у горизонта (~34') + радиус Солнца (~16') дают в сумме 50' = 0.8333° = 83 сотых градуса.
    // Официальный зенит заката: 90° + 0.83° = 90.83° (в коде 9083).
    // Для сумерек к базовым углам погружения (6°, 12°, 18°) также добавляется эта рефракция.
    int32_t zenith_scaled = 9083; 
    switch (type) {
        case TwilightType::Civil:        zenith_scaled = 9683;  break; // 90° + 6° + 0.83°
        case TwilightType::Nautical:     zenith_scaled = 10283; break; // 90° + 12° + 0.83°
        case TwilightType::Astronomical: zenith_scaled = 10883; break; // 90° + 18° + 0.83°
        default:                         zenith_scaled = 9083;  break;
    }

    // 4. ЛАМБДА-ФУНКЦИЯ ДЛЯ ВЫЧИСЛЕНИЯ ПОЛОВИНЫ ДНЯ (Восход / Закат)
    // Аргумент is_sunrise: true — расчет утра, false — расчет вечера
    auto calculate_half_day = [&](bool is_sunrise, uint32_t& out_time) -> bool {
        
        // Приблизительное время события в сутках (масштаб 10000)
        int32_t t_fp = is_sunrise ? (N * 10000 + (60000 - lng_hour_fp) / 24) : (N * 10000 + (180000 - lng_hour_fp) / 24);

        // Средняя аномалия Солнца (M) в сотых долях градуса
        // Формула: M = 0.9856 * t - 3.2891
        int32_t M_scaled = (9856 * t_fp) / 1000000 - 328; 
        
        // Истинная долгота Солнца (L)
        // Формула: L = M + 1.916 * sin(M) + 0.020 * sin(2M) + 282.634
        int32_t sinM = fp_sin(M_scaled);
        int32_t sin2M = fp_sin(M_scaled * 2);
        int32_t L_scaled = M_scaled + (1916 * sinM) / 10000 + (20 * sin2M) / 10000 + 28263;
        L_scaled %= 36000;
        if (L_scaled < 0) L_scaled += 36000;

        // Прямое восхождение Солнца (RA)
        // Формула: tan(RA) = 0.91746 * tan(L)
        int32_t tanL = (fp_sin(L_scaled) * 10000) / fp_cos(L_scaled);
        int32_t y_ra = (9175 * tanL) / 10000;
        int32_t RA_scaled = fp_atan2(y_ra, 10000);
        
        // Коррекция квадранта для RA, чтобы вернуть его в диапазон L
        int32_t L_quad = (L_scaled / 9000) * 9000;
        int32_t RA_quad = (RA_scaled / 9000) * 9000;
        RA_scaled = RA_scaled + (L_quad - RA_quad);
        int32_t RA_hour_fp = (RA_scaled * 100) / 15; // Перевод угла в часовую меру (масштаб 10000)

        // Склонение Солнца (Dec)
        // Формула: sin(Dec) = 0.39782 * sin(L)
        int32_t sinDec = (3978 * fp_sin(L_scaled)) / 10000;
        int32_t cosDec = fp_cos(fp_acos(sinDec)); // Нахождение косинуса через acos

        // Вычисление локального часового угла Солнца (cosH)
        // Формула: cos(H) = (cos(zenith) - sin(Dec)*sin(lat)) / (cos(Dec)*cos(lat))
        // Используем тип int64_t для промежуточного умножения, чтобы избежать переполнения регистра STM32
        int64_t num = (int64_t)fp_cos(zenith_scaled) * 10000 - (int64_t)sinDec * fp_sin(lat_scaled);
        int64_t den = (int64_t)cosDec * fp_cos(lat_scaled);
        int32_t cosH_fp = (num * 10000) / den; // Масштаб результата 10000

        // ЕСЛИ cosH вылетел за пределы [-1; 1], значит Солнце в этих широтах 
        // не достигает указанного зенита (явление Полярного дня или Полярной ночи)
        if (cosH_fp > 10000 || cosH_fp < -10000) return false;

        // Нахождение часового угла H в сотых долях градуса
        int32_t H_scaled = fp_acos(cosH_fp);
        if (is_sunrise) H_scaled = 36000 - H_scaled; // Для утра инвертируем угол
        int32_t H_hour_fp = (H_scaled * 100) / 15;   // Перевод угла в часовую меру (масштаб 10000)

        // Расчет всемирного времени события (UT) в часах
        // Формула: T = H + RA - 0.06571 * t - 6.622
        int32_t T_fp = H_hour_fp + RA_hour_fp - (657 * t_fp) / 100000 - 66220;
        int32_t UT_fp = T_fp - lng_hour_fp; // Перевод местного времени в UTC
        
        // Нормализация UT к диапазону [0, 24 часа)
        UT_fp %= 240000;
        if (UT_fp < 0) UT_fp += 240000;

        // ПЕРЕВОД РЕЗУЛЬТАТА В СЕКУНДЫ UNIX TIME
        // UT_fp содержит часы с масштабом 10000. Умножаем на 3600 секунд и делим на 10000.
        uint32_t seconds_of_day = ((uint64_t)UT_fp * 3600) / 10000;
        
        // Начало текущих суток (00:00:00 UTC) для переданного Unix Time
        uint32_t start_of_day = (unix_time / 86400) * 86400;
        
        // Итоговое локальное время: Начало суток UTC + Смещение секунд внутри дня UTC + Сдвиг часового пояса
        out_time = start_of_day + seconds_of_day + tz_offset_seconds;
        return true;
    };

    // Вычисляем утреннюю и вечернюю метку по отдельности
    bool sunrise_ok = calculate_half_day(true, result.start_time);
    bool sunset_ok  = calculate_half_day(false, result.end_time);

    // Если обе фазы успешно рассчитаны (нет полярной ночи/дня)
    if (sunrise_ok && sunset_ok) {
        result.is_valid = true;
    }
    return result;
}
