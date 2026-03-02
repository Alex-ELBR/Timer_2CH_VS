#include <eEEPROM.h>

/// @brief ////////////////////////////////////////////////////////////////////////
/// @param i2c_obj 
/// @param address 
eEEPROM::eEEPROM(I2C_HandleTypeDef *i2c_obj, uint16_t address)
{
    _i2c_bus = i2c_obj;
    _dev_address = address;
}



/// @brief запись настроек таймера в EEPROM /////////////////////////////////////////
/// @param timer 
/// @return если успешно - HAL_OK
HAL_StatusTypeDef eEEPROM::write_timer_eeprom(eTimer &timer)
{
    uint16_t tim_addr;
    timer_setings_t *ptr_settings_buffer = (timer_setings_t*)_settings_buffer;

    tim_addr = timer.get_timer_settings_address();

    timer.get_timer_settings(ptr_settings_buffer);
       
    while(HAL_I2C_Mem_Write(_i2c_bus, (_dev_address << 1), tim_addr, I2C_MEMADD_SIZE_16BIT, _settings_buffer, sizeof(timer_setings_t), HAL_I2C_ERROR_TIMEOUT ) != HAL_OK)
    {
        return HAL_TIMEOUT;
    } 
    while(HAL_I2C_IsDeviceReady(_i2c_bus, _dev_address, 3, HAL_I2C_ERROR_TIMEOUT) != HAL_OK) 
    {
         return HAL_TIMEOUT; 
    }

    return HAL_OK;
}


/// @brief чтение настроек таймера из EEPROM ////////////////////////////////////////
/// @param timer 
/// @return если успешно - HAL_OK
HAL_StatusTypeDef eEEPROM::read_timer_eeprom(eTimer &timer)
{
    uint16_t tim_addr;
    timer_setings_t *ptr_settings_buffer = (timer_setings_t*)_settings_buffer;

    tim_addr = timer.get_timer_settings_address();

    while(HAL_I2C_Mem_Read(_i2c_bus, (_dev_address << 1), tim_addr, I2C_MEMADD_SIZE_16BIT, _settings_buffer, sizeof(timer_setings_t), HAL_I2C_ERROR_TIMEOUT) != HAL_OK)
    {
        return HAL_TIMEOUT;
    }    


    timer.set_timer_settings(ptr_settings_buffer);

    return HAL_OK;
}

/// @brief запись настроек канала в EEPROM //////////////////////////////////////////////////////////////////////////////////////
/// @param channel 
/// @return если успешно - HAL_OK
HAL_StatusTypeDef eEEPROM::write_channel_eeprom(eChannel &channel)
{
    uint16_t ch_addr;
    channel_settings_t *ptr_settings_buffer = (channel_settings_t*)_settings_ch_buffer;

    ch_addr = channel.get_channel_settings_address();
    channel.get_channel_settings(ptr_settings_buffer);
    
    while(HAL_I2C_Mem_Write(_i2c_bus, (_dev_address << 1), ch_addr, I2C_MEMADD_SIZE_16BIT, _settings_buffer, sizeof(channel_settings_t), HAL_I2C_ERROR_TIMEOUT ) != HAL_OK)
    {
        return HAL_TIMEOUT;
    } 
    while(HAL_I2C_IsDeviceReady(_i2c_bus, _dev_address, 3, HAL_I2C_ERROR_TIMEOUT) != HAL_OK) { return HAL_TIMEOUT; }

    return HAL_OK;
}

/// @brief чтение настроек канала из EEPROM //////////////////////////////////////////////////////////////////////////////////////
/// @param channel 
/// @return если успешно - HAL_OK
HAL_StatusTypeDef eEEPROM::read_channel_eeprom(eChannel &channel)
{
    uint16_t ch_addr;
    channel_settings_t *ptr_settings_buffer = (channel_settings_t*)_settings_ch_buffer;

    ch_addr = channel.get_channel_settings_address();

    while(HAL_I2C_Mem_Read(_i2c_bus, (_dev_address << 1), ch_addr, I2C_MEMADD_SIZE_16BIT, _settings_ch_buffer, sizeof(channel_settings_t), HAL_I2C_ERROR_TIMEOUT) != HAL_OK)
    {
        return HAL_TIMEOUT;
    }    

    channel.load_channel_settings(ptr_settings_buffer);
    return HAL_OK;
}

