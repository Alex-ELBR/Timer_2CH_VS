#ifndef EDISPATCHER_H_INCLUDED
#define EDISPATCHER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"
#include <vector>

using std::vector;

const uint32_t ETASK_TIMEOUT_MS = (uint32_t)5000;

/// @brief //////////////////////////////////////////////////////////////////////
class ETask
{
    public:
        ETask(void callback_function(), uint32_t period_ms);
        void execution(void);
        void suspend(void);
        void resume(void);

    private:
        uint32_t _delay_ms;
        uint32_t _timeKeep;
        bool _enable;
        void (*callback_ptr)();
};



/// @brief //////////////////////////////////////////////////////////////////////
class eDispatcher
{
    public:
        eDispatcher();
        void add_task(void callback_function(), uint32_t period_ms);
        void execution(void);

        void tasks_timeout_check(void callback_exeption());
        uint8_t get_current_task();

    private:
        vector<ETask> _tasks;
        uint32_t _timeout;
        size_t _current_task;

};




#endif //EDISPATCHER_H_INCLUDED