#ifndef __MENU_MAIN_H
#define __MENU_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include "config_device.h"
#include <time_common.h>

#include <eMenu.h>





extern eMenu mainMenu;

void start_config_rtc(eMenu::Context& ctx);

using Button = eButton::ButtonNumber; 


bool menu_config_rtc(Button button, eMenu::Context& ctx); // настройка времени (часыб минуты)
bool menu_config_loc_lon(Button button, eMenu::Context& ctx); // настройка географических координат - долгота
bool menu_config_loc_lat(Button button, eMenu::Context& ctx); // настройка географических координат - широта
bool menu_config_loc_tz(Button button, eMenu::Context& ctx); // настройка географических координат - часовой пояс







#endif //__MENU_MAIN_H