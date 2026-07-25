#ifndef __MAIN_LOOP_H
#define __MAIN_LOOP_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <config_device.h>
#include <eDispatcher.h>
#include <eDisplay.hpp>
#include <eLed.h>
#include <eDS1338.hpp>
#include <eButton.h>
#include <eChannel.h>
#include <eEEPROM.h>
#include <eMenu.h>

#include "main.h"
#include <menu_main.h>



enum _STEP_CASE_
{
    STARTUP_LOAD = 0,
    NORMAL_WORK,
    CONFIGURATION,
    CLEAR_CONFIG,
};



enum _SCREEN_CONF_RTC_
{
  SCREEN_CONF_HOUR,
  SCREEN_CONF_MINUTE,
  SCREEN_CONF_DAY,
  SCREEN_CONF_DATE,
  SCREEN_CONF_MONTH,
  SCREEN_CONF_YEAR

};

enum STAT_CASE
{


};


struct tree_node_t
{
  int field;           // поле данных
  struct tree_node_t *left;  // левый потомок
  struct tree_node_t *right; // правый потомок

};



void main_loop(void);



#endif // __MAIN_LOOP_H