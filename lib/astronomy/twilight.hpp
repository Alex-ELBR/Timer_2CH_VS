#pragma once

#include <stdint.h>

/**
 * @brief Перечисление доступных типов сумерек и граничных состояний Солнца.
 */
enum class TwilightType {

    Official,        // Официальный Восход / Закат (Центр Солнца пересекает горизонт)
    Civil,           // Гражданские сумерки (Светло без ламп, Солнце на 6° ниже горизонта)
    Nautical,        // Навигационные сумерки (Видны контуры предметов и навигационные звезды, 12°)
    Astronomical     // Астрономические сумерки (Полная темнота для телескопов, Солнце на 18° ниже горизонта)
};

/**
 * @brief Структура для хранения результатов расчета.
 */
struct TwilightResult {

    uint32_t start_time; // Локальный Unix timestamp утреннего события (начало сумерек или восход)
    uint32_t end_time;   // Локальный Unix timestamp вечернего события (конец сумерек или закат)
    bool is_valid;       // Флаг валидности. false, если в этой широте полярный день или полярная ночь
    bool polar_day;      // если is_valid = false -> это полярный день?
    bool polar_night;    // если is_valid = false -> это полярная ночь?
};

/**
 * @brief Функция расчета локального времени сумерек/восходов/закатов без использования float/double.
 * 
 * @param unix_time Текущее базовое время в формате UTC Unix timestamp (секунды с 01.01.1970).
 * @param latitude Широта в градусах
 * @param latitude Долгота в градусах
 * @param time_zone часовой пояс (например: UTC+3, UTC+5:30, UTC-4).
 * @param type Требуемый тип сумерек.
 * @return TwilightResult Структура с локальными метками времени (уже включают сдвиг таймзоны).
 */
TwilightResult calculate_twilight(uint32_t unix_time, float latitude, float longitude, float time_zone, TwilightType type);