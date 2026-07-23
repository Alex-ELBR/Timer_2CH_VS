#pragma once

#include "stm32f1xx_hal.h"

class eGPS {
public:
    eGPS(GPIO_TypeDef* rxPort, uint16_t rxPin, TIM_HandleTypeDef* htim, I2C_HandleTypeDef* i2c, int8_t timeZoneHours = 3);

    void init();
    void handleRxPinInterrupt();
    void handleTimerInterrupt();

    // Операции чтения данных часов
    void periodic(void);
    
    uint32_t get_unix_time(void);
    uint8_t get_hour(void);
    uint8_t get_minute(void);
    bool get_sec_comma(void);
    uint8_t get_day(void);
    uint8_t get_date(void);
    uint8_t get_month(void);
    uint16_t get_year(void);
    
    uint32_t get_sec_only_day(void);
    uint32_t get_sec_week(void);

    // Географические данные
    int16_t get_latitude_degree(void);
    int16_t get_latitude_min(void);
    int16_t get_latitude_sec(void);
    int16_t get_longitude_degree(void);
    int16_t get_longitude_min(void);
    int16_t get_longitude_sec(void);
    int16_t get_timezone(void);

    bool isGpsFixed() const;
    void setTimeZone(int8_t hours);

private:
    static constexpr uint16_t RX_BUF_SIZE = 96;
    static constexpr uint32_t SYNC_INTERVAL_SEC = 24 * 3600;
    static constexpr uint16_t DS1338_ADDR = 0x68 << 1;

    GPIO_TypeDef*      m_rxPort;
    uint16_t           m_rxPin;
    TIM_HandleTypeDef* m_htim;
    I2C_HandleTypeDef* m_hi2c;

    int32_t  m_timeZoneOffset;
    double   m_latitude;
    double   m_longitude;
    bool     m_isGpsValid;

    char     m_nmeaBuf[RX_BUF_SIZE];
    uint16_t m_bufIdx;
    uint32_t m_lastSyncTick;
    
    uint8_t  m_bitIdx;
    uint8_t  m_rxByte;
    bool     m_isReceiving;
    bool     m_secComma;

    struct ClockCache {
        uint32_t unix_time;
        uint32_t sec_only_day;
        uint32_t sec_week;
        uint16_t year;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
        uint8_t day;
        uint8_t date;
        uint8_t month;
    } m_timeCache;

    uint8_t bcd2dec(uint8_t val) const { return ((val / 16 * 10) + (val % 16)); }
    uint8_t dec2bcd(uint8_t val) const { return ((val / 10 * 16) + (val % 10)); }

    void enableRtcOscillator();
    void readTimeFromDs1338(uint8_t &y, uint8_t &m, uint8_t &d, uint8_t &h, uint8_t &mn, uint8_t &s, uint8_t &wday);
    void writeTimeToDs1338(uint8_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t mn, uint8_t s, uint8_t wday);
    void convertFloatToDms(double decimalValue, int16_t &deg, int16_t &min, int16_t &sec) const;
    void parseNmeaLight(char* str);
    double convertNmeaToDecimal(double rawDegreeMinutes, char cardinal);
    uint32_t dateToUnix2000(uint8_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t min, uint8_t s) const;
    void unix2000ToDate(uint32_t unix2000, uint8_t &y, uint8_t &m, uint8_t &d, uint8_t &h, uint8_t &min, uint8_t &s) const;
    uint8_t getDayOfWeekCalc(int y, int m, int d) const;
};
