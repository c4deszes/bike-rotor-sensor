#include "tiny_sil/sch.hpp"

extern "C" {
    #include "hal/sch.h"

    void _dummy_task(void);
}

using namespace SIL;

Scheduler global_scheduler = Scheduler();

void sch_init(void) {
    
}

void sch_enter(void) {
    
}

void _dummy_task(void) {
    /** Do nothing */
}

void sch_task1ms(void) __attribute__((weak, alias("_dummy_task")));

void sch_task10ms(void) __attribute__((weak, alias("_dummy_task")));
