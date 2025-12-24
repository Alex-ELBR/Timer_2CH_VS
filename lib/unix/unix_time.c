#include "unix_time.h"

/*************************************************************************/
#define SEC_A_DAY   86400
#define ONE_HOUR    3600    /** ќдин час, выраженный в секундах */
#define ONE_DEGREE  3600L   /** ”гловой градус, выраженный в угловых секундах */
#define ONE_DAY     86400   /** ќдин день, выраженный в секундах */



void unix_to_date(const uint32_t unix_time, real_time_t *timeptr)
{
	uint32_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint32_t time;

	time = unix_time % SEC_A_DAY;
	a = ((unix_time + 43200) / (86400 >> 1)) + (2440587 << 1) + 1;
	a >>= 1;
	timeptr->day = a % 7;
	a += 32044;
	b = (4*a+3) / 146097;
	a = a - (146097 * b) / 4;
	c = (4 * a + 3) / 1461;
	a = a - (1461 * c) / 4;
	d = (5 * a + 2) / 153;
	timeptr->date = a - (153 * d + 2) / 5 + 1;
	timeptr->month = (d + 3 - 12 * (d / 10));
	timeptr->year = 100 * b + c - 4800 + (d / 10);
	timeptr->hour = (time / 3600);
	timeptr->minute = (time % 3600) / 60;
	timeptr->second = (time % 3600) % 60;
}


uint32_t date_to_unix(const real_time_t *timeptr)
{
   	uint8_t a;
	uint16_t y;
	uint8_t m;
	uint32_t Uday;
	uint32_t unix_time;

	a = ((14 - timeptr->month) / 12);
	y = timeptr->year + 4800 - a;
	m = timeptr->month + (12 * a) - 3;
	Uday = (timeptr->date + ((153 * m + 2) / 5) + 365 * y + (y / 4) - (y / 100) + (y / 400) - 32045) - 2440588;
	unix_time = Uday * 86400;
	unix_time += timeptr->second + timeptr->minute * 60 + timeptr->hour *3600;

	return unix_time;
}


uint8_t unix_to_day(const uint32_t unix_time)
{
    uint32_t a;
	a = ((unix_time + 43200) / (86400 >> 1)) + (2440587 << 1) + 1;
	a >>= 1;
	
	return (a % 7);	
}

/**************************************************************************************************************************************
***************************************************************************************************************************************/
uint8_t is_leap_year(int16_t year)
{
    div_t d;

    /* year must be divisible by 4 to be a leap year */
    if (year & 3)
	{
        return 0;
	}

    /* If theres a remainder after division by 100, year is not divisible by 100 or 400 */
    d = div(year, 100);

    if (d.rem)
	{
        return 1;
	}

    /* If the quotient is divisible by 4, then year is divisible by 400 */
    if ((d.quot & 3) == 0)
	{
        return 1;
	}

    return 0;
}