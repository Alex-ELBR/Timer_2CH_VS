#ifndef __eEEPROM_H
#define __eEEPROM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include "unix_time.h"
#include <eTimer.h>
#include <eChannel.h>


class eEEPROM
{
    public:
        eEEPROM(I2C_HandleTypeDef *i2c_obj, uint16_t address);

        HAL_StatusTypeDef write_timer_eeprom(eTimer &timer);
        HAL_StatusTypeDef read_timer_eeprom(eTimer &timer);

        HAL_StatusTypeDef write_channel_eeprom(eChannel &channel);
        HAL_StatusTypeDef read_channel_eeprom(eChannel &channel);
        
    private:
        I2C_HandleTypeDef *_i2c_bus;
        uint16_t _dev_address;
        uint8_t _settings_buffer[sizeof(timer_setings_t)] = {0};

};




#endif //__eEEPROM_H