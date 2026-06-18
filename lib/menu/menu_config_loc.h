#ifndef __MENU_CONFIG_LOC_H
#define __MENU_CONFIG_LOC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include <eButton.h>
#include <eDisplay.h>
#include <eRTC.h>

enum _SCREEN_CONF_LOC_
{
    SCREEN_CONF_LOC_LON,
    SCREEN_CONF_LOC_LAT,
    SCREEN_CONF_LOC_TMZ,

    LAST_MENU_CONF_LOC,
    ENTER_CONFIG_LOC
};

enum _STEP_CONFIG_LOC_LON_
{
    STEP_CONFIG_LON_DEG,
    STEP_CONFIG_LON_MIN,
    STEP_CONFIG_LON_SEC,
    STEP_APPLY_CONFIG_LOC_LON
};

enum _STEP_CONFIG_LOC_LAT_
{
    STEP_CONFIG_LAT_DEG,
    STEP_CONFIG_LAT_MIN,
    STEP_CONFIG_LAT_SEC,
    STEP_APPLY_CONFIG_LOC_LAT
};

enum _STEP_CONFIG_TIMEZONE_
{
    STEP_CONFIG_TIMEZONE,
    STEP_APPLY_CONFIG_TIMEZONE
};



typedef struct 
{
    bool enable;
    uint32_t time_on;
    uint32_t time_off;
}__attribute__((__packed__)) ch_timer_t;

typedef struct 
{
    uint8_t mode;
    ch_timer_t ch_timer[100];

}__attribute__((__packed__)) channel_t;



bool menu_config_loc(uint16_t button, Display& displ, eRTC& rtc);
bool menu_config_loc_lon(uint16_t button);
bool menu_config_loc_lat(uint16_t button);
bool menu_config_loc_timezone(uint16_t button);



#endif //__MENU_CONFIG_LOC_H