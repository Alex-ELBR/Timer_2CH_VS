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


// Режимы работы прибора
enum class WorkMode : uint8_t
{
    STARTUP_LOAD,     // Начальная загрузка 
    NORMAL_WORK,      // Обычная работа
    CONFIGURATION,    // Настройка прибора    
};

// Экраны основного режима работы
enum class ScreensNormalWork : uint8_t
{
   MAIN_SCREEN,
   OTHER_SCREEN,

   //..............
   COUNT
};



void main_loop(void);



#endif // __MAIN_LOOP_H