#include <eChannel.h>


eChannel::eChannel(void)
{
    _channel_number = _channel_obj_count;
    ++_channel_obj_count;
}

uint16_t eChannel::get_channel_settings_address()
{
    uint16_t __timer_obj_count = eTimer::get_timer_obj_count();
    return (sizeof(channel_settings_t) * _channel_number) + (sizeof(timer_setings_t) * __timer_obj_count);;
}

uint8_t eChannel::get_channel_function(void)
{
    return _channel_func;
}

void eChannel::set_channel_function(const uint8_t channel_func)
{
    _channel_func = channel_func;
}

void eChannel::get_channel_settings(channel_settings_t *settings)
{
    settings->channel_func = _channel_func;
}

void eChannel::set_channel_settings(const channel_settings_t *settings)
{
    _channel_func = settings->channel_func;
    // .. OTHER SETTINGS ...
}