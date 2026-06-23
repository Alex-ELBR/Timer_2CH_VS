#ifndef __MENU_MAIN_H
#define __MENU_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include <time_common.h>

#include "menu_config_loc.h"
#include <eMenu.h>





extern eMenu mainMenu;


bool menu_config_rtc(eButton::pressed_but_t button, eMenu::Context& ctx);
void start_config_rtc(eMenu::Context& ctx);






#endif //__MENU_MAIN_H