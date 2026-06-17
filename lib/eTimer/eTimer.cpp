#include <eTimer.h>
#include <type_traits>

uint16_t eTimer::get_timer_obj_count() { return _timer_number; }


eTimer::eTimer(void)
{
    _timer_settings_addr = (uint16_t)(sizeof(timer_setings_t) * _timer_number);
    _tim_num = _timer_number;
    ++_timer_number;

    _tim_enable = false;
    _time_off = 0;
    _time_on = 0;
    _real_time_on = {0};
    _real_time_off = {0};
}

uint16_t eTimer::get_timer_settings_address() { return _timer_settings_addr; }

void eTimer::get_timer_settings(timer_setings_t *settings)
{
    settings->enable = _tim_enable;
    settings->time_on = _time_on;
    settings->time_off = _time_off;
}


void eTimer::set_enable(bool action) { _tim_enable = action; }

void eTimer::set_time_on(timer_real_time_t real_time) { _time_on = real_time_to_unix(real_time); }
void eTimer::set_time_on(uint32_t time_on) 
{ 
    _time_on = time_on; 
    _real_time_on = unix_to_real_time(time_on);
}

void eTimer::set_time_off(timer_real_time_t real_time) { _time_off = real_time_to_unix(real_time); }
void eTimer::set_time_off(uint32_t time_off) 
{
     _time_off = time_off; 
    _real_time_off = unix_to_real_time(time_off);
}

timer_real_time_t eTimer::get_real_time_on(void) { return _real_time_on; }
timer_real_time_t eTimer::get_real_time_off(void) { return _real_time_off; }


void eTimer::change_parameter(uint8_t parameter_name, uint16_t op)
{
   
    switch(parameter_name)
    {
        case CHANGE_TIM_ENABLE:
        {
            change_operation(&_tim_enable, op, false, true);
        }; break;

        case CHANGE_TIM_ON_HOUR:
        {
            change_operation(&_real_time_on.hour, op, 0, 23);
        }; break;

        case CHANGE_TIM_ON_MIN:
        {
            change_operation(&_real_time_on.minute, op, 0, 59);
        }; break;

        case CHANGE_TIM_ON_DAY:
        {
            change_operation(&_real_time_on.day, op, MONDAY, ALL_DAY);
        }; break;

        case CHANGE_TIM_OFF_HOUR:
        {
            change_operation(&_real_time_off.hour, op, 0, 23);
        }; break;

        case CHANGE_TIM_OFF_MIN:
        {
            change_operation(&_real_time_off.minute, op, 0, 59);
        }; break;

        case CHANGE_TIM_OFF_DAY:
        {
            change_operation(&_real_time_off.day, op, MONDAY, SUNDAY);
        }; break;

    }
}

void eTimer::set_timer_settings(void)
{
    if(_real_time_on.day == ALL_DAY)
    {
        _real_time_on.day = 0;
        _real_time_off.day = 0;
    }
    set_time_on(_real_time_on);
    set_time_off(_real_time_off);

}

void eTimer::set_timer_settings(const timer_setings_t *settings)
{
    _tim_enable = settings->enable;
    set_time_on(settings->time_on);
    set_time_off(settings->time_off);

    // .. OTHER SETTINGS ...
}

timer_real_time_t eTimer::unix_to_real_time(const uint32_t time)
{
    timer_real_time_t real_time;
    real_time.day = (uint8_t)(time / 86400);
    real_time.hour = (uint8_t)((time % 86400) / 3600);
    real_time.minute = (uint8_t)(((time % 86400) % 3600) / 60);
    real_time.second = 0; //(uint8_t)((((time % 86400) % 3600) % 60) / 60);
    return real_time;
}

uint32_t eTimer::real_time_to_unix(const timer_real_time_t real_time)
{
   return(uint32_t)((uint32_t)(real_time.day) * 86400) \
                + ((uint32_t)(real_time.hour) * 3600) \
                + ((uint32_t)(real_time.minute) * 60) \
                + ((uint32_t)(real_time.second));
}


bool eTimer::get_timer_enable(void)
{
    return _tim_enable;
}



/*** служебные функции ***********************************************************************************/
template <typename PARAM, typename OP, typename LMX, typename LMN> 
void eTimer::change_operation(PARAM *ptr_param, OP op, LMN limit_min, LMX limit_max)
{
    switch(op)
    {
        case PLUS:
        {
            
            if constexpr(std::is_same<PARAM, bool>::value)
            {
                (*ptr_param) ^= true;
            }
            else
            {
                if((*ptr_param) < limit_max) ++(*ptr_param);
                else (*ptr_param) = limit_min;
            }
                
        }; break;

        case MINUS:
        {
            
            if constexpr(std::is_same<PARAM, bool>::value)
            {
                (*ptr_param) ^= true;
            }
            else
            {
                if((*ptr_param) > limit_min) --(*ptr_param);
                else (*ptr_param) = limit_max;
            }
                
        }; break;

        case APPLY_CHANGE_TIM:
        {
            if(_real_time_on.day == ALL_DAY)
            {
                _real_time_on.day = 0;
                _real_time_off.day = 0;
            }
            set_time_on(_real_time_off);
            set_time_off(_real_time_on);

            //запись в EEPROM                 
        }; break;
    }
}