#ifndef __ECHANNEL_H
#define __ECHANNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include "unix_time.h"
#include <eTimer.h>

typedef struct 
{
    uint8_t channel_func;

}channel_settings_t;



class eChannel
{
    public:
        eChannel(void);
        eTimer timers[TIMER_AMOUNT];

        uint8_t get_channel_function(void);
        void set_channel_function(const uint8_t channel_func);

        uint16_t get_channel_settings_address();
        void get_channel_settings(channel_settings_t *settings);
        void set_channel_settings(const channel_settings_t *settings);




    private:
        uint8_t _channel_func;
        uint16_t _channel_settings_addr;
        static inline uint16_t _channel_obj_count={};
        uint16_t _channel_number;


};







#endif //__ECHANNEL_H