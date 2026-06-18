#ifndef __MENU_CONFIG_CHANNEL_H
#define __MENU_CONFIG_CHANNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include <eButton.h>
#include <eDisplay.h>
#include <eRTC.h>
#include <eChannel.h>
#include <eEEPROM.h>

enum _SCREEN_CONF_CHNL_
{
    SCREEN_CONF_CH_NUM,

    LAST_MENU_CONF_CHNL,
    ENTER_CONFIG_CHNL
};

enum _SCREEN_CONF_CH_
{
    SCREEN_CONF_CH_FUNC,
    SCREEN_CONF_CH_TIM,

    LAST_MENU_CONF_CH,
    ENTER_CONFIG_CH
};

enum _SCREEN_CONF_TIM_
{
    SCREEN_CONF_TIM,

    LAST_MENU_CONF_TIM,
    ENTER_CONFIG_TIM
};

enum _SCREEN_CONF_CH_FUNC_
{
    SCREEN_CH_FUNC_OFF,
    SCREEN_CH_FUNC_WEEK,
    SCREEN_CH_FUNC_SUN,
    SCREEN_CH_FUNC_CIVIL,

    LAST_MENU_CH_FUNC,
    ENTER_CONFIG_CH_FUNC
};

enum _SCREEN_CONF_SELECTED_TIM_
{
    SCREEN_CONF_TIM_ENABLE,
    SCREEN_CONF_TIM_TIME_OFF,
    SCREEN_CONF_TIM_DAY_OFF,
    SCREEN_CONF_TIM_TIME_ON,
    SCREEN_CONF_TIM_DAY_ON,

    LAST_MENU_CONF_SELECTED_TIM,
    ENTER_CONFIG_SELECTED_TIM
};

enum _STEP_CONFIG_SELECTED_TIM_
{
    STEP_CONFIG_TIM_ENABLE,

    STEP_CONFIG_HOUR_ON,
    STEP_CONFIG_MIN_ON,
    STEP_CONFIG_DAY_ON,

    STEP_CONFIG_HOUR_OFF,
    STEP_CONFIG_MIN_OFF,
    STEP_CONFIG_DAY_OFF,

    STEP_APPLY_CONFIG_SELECTED_TIM
};


bool menu_config_chnl(uint16_t button, Display& displ, eRTC& rtc);
bool menu_config_ch(uint16_t button, uint8_t ch_num);
bool menu_config_ch_func(uint16_t button, uint8_t ch_num);
bool menu_config_ch_tim(uint16_t button, uint8_t ch_num);
bool menu_config_ch_selected_tim(uint16_t button, uint8_t ch_num, uint8_t tim_num);



#endif //__MENU_CONFIG_CHANNEL_H