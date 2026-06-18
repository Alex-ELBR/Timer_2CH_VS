#ifndef __MENU_CONFIG_RTC_H
#define __MENU_CONFIG_RTC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include <eButton.h>
#include <eDisplay.h>
#include <eRTC.h>
#include <eMenu.h>


enum _STEP_CONFIG_RTC_
{
    STEP_CONFIG_HOUR,
    STEP_CONFIG_MIN,
    STEP_CONFIG_YEAR,
    STEP_CONFIG_MONTH,
    STEP_CONFIG_DATE,

    STEP_APPLY_CONFIG_RTC
};




extern eMenu menuConfigRTC;


#endif //__MENU_CONFIG_RTC_H