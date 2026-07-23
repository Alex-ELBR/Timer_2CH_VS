#include "eGPS.hpp"
#include <cstring>
#include <cstdlib>

eGPS::eGPS(GPIO_TypeDef* rxPort, uint16_t rxPin, TIM_HandleTypeDef* htim, I2C_HandleTypeDef* i2c, int8_t timeZoneHours)
    : m_rxPort(rxPort), m_rxPin(rxPin), m_htim(htim), m_hi2c(i2c), m_timeZoneOffset(timeZoneHours * 3600),
      m_latitude(0.0), m_longitude(0.0), m_isGpsValid(false), m_bufIdx(0), m_lastSyncTick(0), 
      m_bitIdx(0), m_rxByte(0), m_isReceiving(false), m_secComma(false), m_timeCache{0} {}

void eGPS::init() { enableRtcOscillator(); }

void eGPS::handleRxPinInterrupt() {
    if (!m_isReceiving && HAL_GPIO_ReadPin(m_rxPort, m_rxPin) == GPIO_PIN_RESET) {
        m_isReceiving = true; m_bitIdx = 0; m_rxByte = 0;
        __HAL_TIM_SET_AUTORELOAD(m_htim, 156 - 1);
        __HAL_TIM_SET_COUNTER(m_htim, 0);
        HAL_TIM_Base_Start_IT(m_htim);
    }
}

void eGPS::handleTimerInterrupt() {
    if (m_isReceiving) {
        if (m_bitIdx == 0) { __HAL_TIM_SET_AUTORELOAD(m_htim, 104 - 1); }
        if (m_bitIdx < 8) {
            uint8_t bit = (HAL_GPIO_ReadPin(m_rxPort, m_rxPin) == GPIO_PIN_SET) ? 1 : 0;
            m_rxByte |= (bit << m_bitIdx); m_bitIdx++;
        } else {
            m_isReceiving = false; HAL_TIM_Base_Stop_IT(m_htim);
            char ch = static_cast<char>(m_rxByte);
            if (ch == '\n' || ch == '\r') {
                if (m_bufIdx > 0) { m_nmeaBuf[m_bufIdx] = '\0'; parseNmeaLight(m_nmeaBuf); m_bufIdx = 0; }
            } else if (m_bufIdx < RX_BUF_SIZE - 1) { m_nmeaBuf[m_bufIdx++] = ch; }
            else { m_bufIdx = 0; }
        }
    }
}

void eGPS::periodic(void) {
    uint8_t y, m, d, h, mn, s, w;
    readTimeFromDs1338(y, m, d, h, mn, s, w);
    
    m_timeCache.hour = h;
    m_timeCache.minute = mn;
    m_timeCache.second = s;
    m_timeCache.day = w;
    m_timeCache.date = d;
    m_timeCache.month = m;
    m_timeCache.year = 2000 + y;
    
    m_timeCache.unix_time = dateToUnix2000(y, m, d, h, mn, s);
    m_timeCache.sec_only_day = (static_cast<uint32_t>(h) * 3600) + (static_cast<uint32_t>(mn) * 60) + s;
    m_timeCache.sec_week = (static_cast<uint32_t>(w) * 86400) + m_timeCache.sec_only_day;

    m_secComma = ((s % 2) == 0);
}

uint32_t eGPS::get_unix_time() { return m_timeCache.unix_time; }
uint8_t  eGPS::get_hour() { return m_timeCache.hour; }
uint8_t  eGPS::get_minute() { return m_timeCache.minute; }
bool     eGPS::get_sec_comma() { return m_secComma; }
uint8_t  eGPS::get_day() { return m_timeCache.day; }
uint8_t  eGPS::get_date() { return m_timeCache.date; }
uint8_t  eGPS::get_month() { return m_timeCache.month; }
uint16_t eGPS::get_year() { return m_timeCache.year; }
uint32_t eGPS::get_sec_only_day() { return m_timeCache.sec_only_day; }
uint32_t eGPS::get_sec_week() { return m_timeCache.sec_week; }

int16_t eGPS::get_latitude_degree() { int16_t dg,mn,sc; convertFloatToDms(m_latitude,dg,mn,sc); return dg; }
int16_t eGPS::get_latitude_min() { int16_t dg,mn,sc; convertFloatToDms(m_latitude,dg,mn,sc); return mn; }
int16_t eGPS::get_latitude_sec() { int16_t dg,mn,sc; convertFloatToDms(m_latitude,dg,mn,sc); return sc; }
int16_t eGPS::get_longitude_degree() { int16_t dg,mn,sc; convertFloatToDms(m_longitude,dg,mn,sc); return dg; }
int16_t eGPS::get_longitude_min() { int16_t dg,mn,sc; convertFloatToDms(m_longitude,dg,mn,sc); return mn; }
int16_t eGPS::get_longitude_sec() { int16_t dg,mn,sc; convertFloatToDms(m_longitude,dg,mn,sc); return sc; }
int16_t eGPS::get_timezone() { return static_cast<int16_t>(m_timeZoneOffset/3600); }
bool    eGPS::isGpsFixed() const { return m_isGpsValid; }
void    eGPS::setTimeZone(int8_t hours) { m_timeZoneOffset = hours * 3600; m_lastSyncTick = 0; }

void eGPS::enableRtcOscillator() {
    uint8_t reg0 = 0; HAL_I2C_Mem_Read(m_hi2c, DS1338_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &reg0, 1, 100);
    if (reg0 & 0x80) { reg0 &= 0x7F; HAL_I2C_Mem_Write(m_hi2c, DS1338_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, &reg0, 1, 100); }
}

void eGPS::readTimeFromDs1338(uint8_t &y, uint8_t &m, uint8_t &d, uint8_t &h, uint8_t &mn, uint8_t &s, uint8_t &wday) {
    uint8_t buf[7];
    if (HAL_I2C_Mem_Read(m_hi2c, DS1338_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, buf, 7, 100) == HAL_OK) {
        s = bcd2dec(buf[0] & 0x7F); mn = bcd2dec(buf[1]); h = bcd2dec(buf[2] & 0x3F);
        wday = bcd2dec(buf[3]) - 1; d = bcd2dec(buf[4]); m = bcd2dec(buf[5] & 0x1F); y = bcd2dec(buf[6]);
    }
}

void eGPS::writeTimeToDs1338(uint8_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t mn, uint8_t s, uint8_t wday) {
    uint8_t buf[7];
    buf[0] = dec2bcd(s) & 0x7F; buf[1] = dec2bcd(mn); buf[2] = dec2bcd(h) & 0x3F;
    buf[3] = dec2bcd(wday + 1); buf[4] = dec2bcd(d); buf[5] = dec2bcd(m); buf[6] = dec2bcd(y);
    HAL_I2C_Mem_Write(m_hi2c, DS1338_ADDR, 0x00, I2C_MEMADD_SIZE_8BIT, buf, 7, 100);
}

void eGPS::convertFloatToDms(double val, int16_t &deg, int16_t &min, int16_t &sec) const {
    bool neg = (val < 0); if (neg) val = -val;
    deg = static_cast<int16_t>(val); double rawMin = (val - deg) * 60.0;
    min = static_cast<int16_t>(rawMin); sec = static_cast<int16_t>((rawMin - min) * 60.0);
    if (neg) deg = -deg;
}

void eGPS::parseNmeaLight(char* str) {
    if (std::strstr(str, "RMC") == nullptr) return;
    int fieldIdx = 0; char *token = str;
    int hour = 0, min = 0, sec = 0, day = 1, month = 1, year = 0;
    double rawLat = 0.0, rawLon = 0.0; char status = 'V', latCard = 'N', lonCard = 'E';
    
    while (token != nullptr) {
        char *next = std::strchr(token, ','); if (next != nullptr) *next = '\0';
        if (token[0] != '\0') {
            switch (fieldIdx) {
                case 1: if (std::strlen(token) >= 6) {
                    hour = (token[0]-'0')*10+(token[1]-'0'); min = (token[2]-'0')*10+(token[3]-'0'); sec = (token[4]-'0')*10+(token[5]-'0');
                } break;
                case 2: status = token[0]; break;
                case 3: rawLat = std::strtod(token, nullptr); break;
                case 4: latCard = token[0]; break;
                case 5: rawLon = std::strtod(token, nullptr); break;
                case 6: lonCard = token[0]; break;
                case 9: if (std::strlen(token) == 6) {
                    day = (token[0]-'0')*10+(token[1]-'0'); month = (token[2]-'0')*10+(token[3]-'0'); year = (token[4]-'0')*10+(token[5]-'0');
                } break;
            }
        } else {
            if (fieldIdx == 2) status = 'V';
        }
        if (next == nullptr) {
            break;
        }
        token = next + 1; 
        fieldIdx++;

    }
    if (status == 'A') {
        m_isGpsValid = true; m_latitude = convertNmeaToDecimal(rawLat, latCard); m_longitude = convertNmeaToDecimal(rawLon, lonCard);
        uint32_t utcTs = dateToUnix2000(year, month, day, hour, min, sec); uint32_t currentTick = HAL_GetTick() / 1000;
        if (m_lastSyncTick == 0 || (currentTick - m_lastSyncTick > SYNC_INTERVAL_SEC)) {
            uint32_t localTs = utcTs + m_timeZoneOffset; uint8_t y_l, m_l, d_l, h_l, mn_l, s_l;
            unix2000ToDate(localTs, y_l, m_l, d_l, h_l, mn_l, s_l);
            writeTimeToDs1338(y_l, m_l, d_l, h_l, mn_l, s_l, getDayOfWeekCalc(2000 + y_l, m_l, d_l));
            m_lastSyncTick = currentTick;
        }
    } else { m_isGpsValid = false; }
}

double eGPS::convertNmeaToDecimal(double raw, char card) {
    int deg = static_cast<int>(raw / 100); double min = raw - (deg * 100); double dec = deg + (min / 60.0);
    if (card == 'S' || card == 'W') { dec = -dec; } return dec;
}

uint32_t eGPS::dateToUnix2000(uint8_t y, uint8_t m, uint8_t d, uint8_t h, uint8_t min, uint8_t s) const {
    if (m <= 2) { m += 12; y -= 1; }
    uint32_t days = d + (153 * m - 457) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 306;
    return (days * 86400) + (h * 3600) + (min * 60) + s;
}

void eGPS::unix2000ToDate(uint32_t u2k, uint8_t &y, uint8_t &m, uint8_t &d, uint8_t &h, uint8_t &min, uint8_t &s) const {
    s = u2k % 60; u2k /= 60; min = u2k % 60; u2k /= 60; h = u2k % 24; uint32_t days = u2k / 24 + 306;
    uint32_t j = (4 * days + 3) / 146097; days -= (146097 * j) / 4;
    uint32_t i = (4 * days + 3) / 1461; days -= (1461 * i) / 4;
    uint32_t k = (5 * days + 2) / 153; d = days - (153 * k + 2) / 5 + 1; m = k + 3; y = 100 * j + i;
    if (m > 12) { m -= 12; y += 1; }
}

uint8_t eGPS::getDayOfWeekCalc(int y, int m, int d) const {
    static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (m < 3) { y -= 1; } return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}
