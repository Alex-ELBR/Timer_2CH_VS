#include <eTask.h>

uint32_t ETask::timeout = 0;

/// @brief конструктор обьекта задачи /////////////////////////////////////////////////////////////////
/// @param callback_function - указатель на функцию, которая будет вызвана после истечения period_ms
/// @param period_ms - периодичность вызова функции в мсек
///////////////////////////////////////////////////////////////////////////////////////////////////////
ETask::ETask (void callback_function(), uint32_t period_ms)
{
    callback_ptr = callback_function;
    delay_ms = period_ms;
    timeKeep = HAL_GetTick();
}

/// @brief функция выполнения задачи, вызывается в главном бесконечном цикле //////////////////////.///
/// @param нет
void ETask::execution(void)
{
    if(HAL_GetTick() - timeKeep > delay_ms)
    {
        timeKeep = HAL_GetTick();
        timeout = 0;
        callback_ptr();
    }
}

/// @brief функция проверки на зависшую задачу, должна вызываться в прерывании SYSTick ////////////////
/// @param callback_exeption указатель на функцию, которая будет вызвана если одна из задач зависла
void ETask::tasks_timeout_check(void callback_exeption())
{
    if(timeout > ETASK_TIMEOUT_MS)
    {
        callback_exeption();
    }
    else
    {
        ++timeout;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////