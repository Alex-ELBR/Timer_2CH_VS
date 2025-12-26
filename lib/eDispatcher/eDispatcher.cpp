#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <eDispatcher.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 
eDispatcher::eDispatcher()
{
    _timeout = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 
/// @param callback_function 
/// @param period_ms 
void eDispatcher::add_task(void callback_function(), uint32_t period_ms)
{
    ETask new_task(callback_function, period_ms);
    _tasks.push_back(new_task);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 
void eDispatcher::execution()
{
    if(!_tasks.empty())
    {
        for(size_t i = 0; i != _tasks.size(); ++i)
        {
            _current_task = i;
            _tasks[i].execution();
            _timeout = 0;
        }
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief функция проверки на зависшую задачу, должна вызываться в прерывании SYSTick ////////////////
/// @param callback_exeption указатель на функцию, которая будет вызвана если одна из задач зависла
void eDispatcher::tasks_timeout_check(void callback_exeption())
{

    if(_timeout > ETASK_TIMEOUT_MS) 
    { 
        callback_exeption(); 
    }
    else { ++_timeout; }
}

uint8_t eDispatcher::get_current_task()
{
    return (uint8_t)_current_task;
}


