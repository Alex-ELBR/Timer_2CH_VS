#include "eDS1338.hpp"












/********************************************************************************************************* */
// Преобразование координат
// Функция преобразования
void eDS1338::convert_coordinate(const loc_data_t *data, float *out_lat, float *out_lon, float *tz) 
{
    if (data == NULL)    { return; }
    if (out_lat != NULL) { *out_lat = dms_to_float(data->lat_deg, data->lat_min, data->lat_sec); }
    if (out_lon != NULL) { *out_lon = dms_to_float(data->lon_deg, data->lon_min, data->lon_sec); }
    if (tz != NULL)      { *tz = (float)data->time_zone; }
}
//->>>>>>>>>>>>
static inline float dms_to_float(int16_t deg, uint8_t min, uint8_t sec)
{
    float decimal = (float)deg;
    float fractional = ((float)min / 60.0f) + ((float)sec / 3600.0f);
        
    // Если градусы отрицательные, дробную часть нужно вычесть
    if (deg < 0) { return decimal - fractional; }
    return decimal + fractional;
}