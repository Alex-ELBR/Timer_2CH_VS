#include <eChannel.h>
#include <type_traits>


eChannel::eChannel(void)
{
    _channel_number = _channel_obj_count + 1;
    ++_channel_obj_count;
    _channel_settings_temp = {0};

}

uint16_t eChannel::get_channel_settings_address() 
{   
    uint16_t __timer_obj_count = eTimer::get_timer_obj_count();
    _channel_settings_addr = (sizeof(channel_settings_t) * _channel_number) + (sizeof(timer_setings_t) * __timer_obj_count);

    return _channel_settings_addr; 
}

uint8_t eChannel::get_channel_function(void)
{
    return _channel_settings_temp.channel_func;
}


void eChannel::get_channel_settings(channel_settings_t *settings)
{
    settings->channel_func = _channel_func;
}

void eChannel::load_channel_settings(const channel_settings_t *settings)
{
    _channel_settings_temp.channel_func = settings->channel_func;
    _channel_func = settings->channel_func;
}


void eChannel::change_parameter(uint8_t parameter_name, uint16_t op)
{
    
    switch(parameter_name)
    {
        case CHANGE_CH_FUNCTION:
        {
            change_operation(&_channel_settings_temp.channel_func, op, CH_FUNC_OFF, CH_FUNC_CIVIL);
        }; break;
    }   
}

/*** служебные функции ***********************************************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////
void eChannel::set_channel_settings(void)
{
    _channel_func = _channel_settings_temp.channel_func;
    // .. OTHER SETTINGS ...
}

/// @brief ////////////////////////////////////////////////////////////////////////////////////////////////
/// @tparam PARAM 
/// @tparam OP 
/// @tparam LMX 
/// @tparam LMN 
/// @param ptr_param 
/// @param op 
/// @param limit_min 
/// @param limit_max /
template <typename PARAM, typename OP, typename LMX, typename LMN> 
void eChannel::change_operation(PARAM *ptr_param, OP op, LMN limit_min, LMX limit_max)
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

        case APPLY_CHANGE_CH:
        {

            //_set_channel_settings();
            //запись в EEPROM                 
        }; break;
    }
}