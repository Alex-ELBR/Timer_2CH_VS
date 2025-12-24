#ifndef ETASK_H_INCLUDED
#define ETASK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f1xx_hal.h"

#define ETASK_TIMEOUT_MS     5000


class ETask
{
    public:
        ETask(void callback_function(), uint32_t period_ms);
        void execution(void);

        static void tasks_timeout_check(void callback_exeption());

    private:
        uint32_t delay_ms;
        uint32_t timeKeep;
        void (*callback_ptr)();

        static uint32_t timeout;

};




#endif //ETASK_H_INCLUDED