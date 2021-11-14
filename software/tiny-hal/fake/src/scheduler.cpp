#include "tiny_sil/scheduler.hpp"

extern "C" {
    #include "hal/sch.h"
}

void sch_init(void) {
    _state = FAKE_SCH_STATE_INIT;
}

void sch_enter(void) {
    _state = FAKE_SCH_STATE_RUNNING;
}

static void _dummy_task(void) {
    /** Do nothing */
}

void sch_task1ms(void) __attribute__((weak, alias("_dummy_task")));

void sch_task10ms(void) __attribute__((weak, alias("_dummy_task")));