#ifndef UNIX_TIME_H_INCLUDED
#define UNIX_TIME_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>

enum _WEEK_DAYS_
{
    MONDAY = 0,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,

    ALL_DAY
};

enum _MOUNTH_
{
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER    
};

typedef struct
{
   uint8_t  second;             /**< seconds after the minute - [ 0 to 59 ] */
   uint8_t sec_comma;              /**< секундная точка */
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

} real_time_t; //структура данных дл¤ часов






void unix_to_date(const uint32_t unix_time, real_time_t *timeptr);
uint32_t date_to_unix(const real_time_t *timeptr);

uint8_t unix_to_day(const uint32_t unix_time);
uint8_t is_leap_year(int16_t year);







#ifdef __cplusplus
}
#endif

#endif /* UNIX_TIME_H_INCLUDED */
