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

void start_config_rtc(eMenu::Context& ctx);


bool menu_config_rtc(eButton::pressed_but_t button, eMenu::Context& ctx); // настройка времени (часыб минуты)
bool menu_config_loc_lon(eButton::pressed_but_t button, eMenu::Context& ctx); // настройка географических координат - долгота
bool menu_config_loc_lat(eButton::pressed_but_t button, eMenu::Context& ctx); // настройка географических координат - широта
bool menu_config_loc_tz(eButton::pressed_but_t button, eMenu::Context& ctx); // настройка географических координат - часовой пояс







#endif //__MENU_MAIN_H