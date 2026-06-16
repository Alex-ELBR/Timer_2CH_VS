/*

#include "twilight.hpp"

void example() {
    // Координаты Москвы: 55.75° N, 37.61° E
    int32_t lat = 5575; 
    int32_t lon = 3761;
    
    // Часовой пояс Москвы: UTC+3:00 -> передаем 300
    int16_t timezone = 300; 
    
    // Пример для Индии (Нью-Дели): UTC+5:30 -> передали бы 530
    // Пример для Нью-Йорка: UTC-5:00 -> передали бы -500

    // Входное время (из RTC или NTP-сервера в формате UTC)
    uint32_t current_utc_unix = 1781524800; 

    // Расчет гражданских сумерек
    TwilightResult result = calculate_twilight(current_utc_unix, lat, lon, timezone, TwilightType::Civil);

    if (result.is_valid) {
        // Эти метки времени уже содержат в себе сдвиг часового пояса.
        // Их можно напрямую использовать для перевода в локальные часы:минуты.
        uint32_t local_morning_start = result.start_time;
        uint32_t local_evening_end = result.end_time;
    }
}

*/