#ifndef __ECHANNEL_H
#define __ECHANNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include <time_common.h>
#include <eTimer.h>

typedef struct 
{
    uint8_t channel_func;

}channel_settings_t;

// типы для параметра "op" функции change_parameter()
enum _TYPE_OP_CH_  
{
    EMPTY_PARAM_CH,
    APPLY_CHANGE_CH
};

// типы для параметра "parameter_name" функции change_parameter()
enum _TYPE_PARAMETER_NAME_CH_
{
    CHANGE_CH_FUNCTION,
  
};

enum _CH_FUNC_
{
    CH_FUNC_OFF = 0,
    CH_FUNC_WEEK,
    CH_FUNC_SUN,
    CH_FUNC_CIVIL,
};

class eChannel
{
    public:
        eChannel(void);
        eTimer timers[TIMER_AMOUNT];

        uint8_t get_channel_function(void);
        uint16_t get_channel_settings_address();
        void get_channel_settings(channel_settings_t *settings);
        void load_channel_settings(const channel_settings_t *settings);
        

        /* Операции изменения настроек канала */
        void change_parameter(uint8_t parameter_name, uint16_t op);
        void set_channel_settings(void);   


    private:
        uint8_t _channel_func;
        uint16_t _channel_settings_addr;
        static inline uint16_t _channel_obj_count{};
        uint16_t _channel_number;
 
        

        channel_settings_t _channel_settings_temp;     
        template <typename PARAM, typename OP, typename LMX, typename LMN> 
        void change_operation(PARAM *ptr_param, OP op, LMN limit_min, LMX limit_max);



};







#endif //__ECHANNEL_H