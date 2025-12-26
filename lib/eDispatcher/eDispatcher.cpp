#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <eDispatcher.h>

eDispatcher::eDispatcher()
{

}

void eDispatcher::add_task(void callback_function(), uint32_t period_ms)
{
    ETask new_task(callback_function, period_ms);
    _tasks.push_back(new_task);
}

void eDispatcher::execution(void)
{
    if(!_tasks.empty())
    {
        for(size_t i = 0; i != _tasks.size(); ++i)
        {
            _timeout = 0;
            _tasks[i].execution();
        }
    }

}

/// @brief функция проверки на зависшую задачу, должна вызываться в прерывании SYSTick ////////////////
/// @param callback_exeption указатель на функцию, которая будет вызвана если одна из задач зависла
void eDispatcher::tasks_timeout_check(void callback_exeption())
{
    if(_timeout > ETASK_TIMEOUT_MS) { callback_exeption(); }
    else { ++_timeout; }
}
