#ifndef __MENU_MAIN_H
#define __MENU_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include <time_common.h>
#include <eButton.h>
#include <eDisplay.h>
#include "menu_config_rtc.h"
#include "menu_config_loc.h"
#include "menu_config_chnl.h"

enum _SCREENS_MAIN_MENU_
{
    SCREEN_CONF_RTC,
    SCREEN_CONF_LOC,
    SCREEN_CONF_CH,

    LAST_MAIN_MENU,
    ENTER_CONFIG
};





bool menu_main(uint16_t button);










#endif //__MENU_MAIN_H