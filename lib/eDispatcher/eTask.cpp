#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <eDispatcher.h>



/// @brief конструктор обьекта задачи /////////////////////////////////////////////////////////////////
/// @param callback_function - указатель на функцию, которая будет вызвана после истечения period_ms
/// @param period_ms - периодичность вызова функции в мсек
///////////////////////////////////////////////////////////////////////////////////////////////////////
eDispatcher::ETask::ETask(void callback_function(), uint32_t period_ms)
{
    callback_ptr = callback_function;
    delay_ms = period_ms;
    timeKeep = HAL_GetTick();
}

/// @brief функция выполнения задачи, вызывается в главном бесконечном цикле //////////////////////.///
/// @param нет
void eDispatcher::ETask::execution(void)
{
    if(HAL_GetTick() - timeKeep > delay_ms)
    {
        timeKeep = HAL_GetTick();
        callback_ptr();
    }
}




///////////////////////////////////////////////////////////////////////////////////////////////////////