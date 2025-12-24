#ifndef __ETIMER_H
#define __ETIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include "unix_time.h"


// типы для параметра "op" функции change_parameter()
enum _TYPE_OP_TIM_  
{
    EMPTY_PARAM,
    APPLY_CHANGE_TIM
};

// типы для параметра "parameter_name" функции change_parameter()
enum _TYPE_PARAMETER_NAME_TIM_
{
    CHANGE_TIM_ENABLE,

    CHANGE_TIM_ON_HOUR,
    CHANGE_TIM_ON_MIN,
    CHANGE_TIM_ON_DAY,
    
    CHANGE_TIM_OFF_HOUR,
    CHANGE_TIM_OFF_MIN,
    CHANGE_TIM_OFF_DAY    
};

typedef struct 
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t day;

} timer_real_time_t;

typedef struct 
{
    uint32_t time_on;
    uint32_t time_off;
    bool enable;
}__attribute__((__packed__)) timer_setings_t;



class eTimer
{
    public:
        eTimer(void);

        void set_enable(bool action);

        void set_time_on(uint32_t time_on);
        void set_time_on(timer_real_time_t real_time);

        void set_time_off(uint32_t time_off);
        void set_time_off(timer_real_time_t real_time);

        timer_real_time_t get_real_time_off(void);
        timer_real_time_t get_real_time_on(void);

        bool get_timer_enable();

        /* Операции изменения данных таймера */
        void change_parameter(uint8_t parameter_name, uint16_t op);

        /* запись настроек таймера */
        static uint16_t get_timer_obj_count();
        uint16_t get_timer_settings_address();
        void set_timer_settings();
        void set_timer_settings(const timer_setings_t *settings);

        /* получить настройки таймера */
        void get_timer_settings(timer_setings_t *settings);




    private:
        bool _tim_enable;                   //включение/отключение таймера
        uint32_t _time_off;                 // время отключения, сек     
        uint32_t _time_on;                  // время отключения, сек

        timer_real_time_t _real_time_off;   //время отключения, день/часы/минуты
        timer_real_time_t _real_time_on;    //время включения, день/часы/минуты

        uint16_t _timer_settings_addr;
        uint16_t _tim_num;

        static inline uint16_t _timer_number{};

        timer_real_time_t unix_to_real_time(const uint32_t time);
        uint32_t real_time_to_unix(const timer_real_time_t real_time);

        template <typename T, typename OP, typename LMX, typename LMN> 
        void change_operation(T *ptr_param, OP op, LMN limit_min, LMX limit_max);

};





#endif //__ETIMER_H