#include "hal/sch.h"
#include "hal/sys.h"

#include <stdint.h>

static volatile uint8_t sch_1ms_flag = 0;
static volatile uint8_t sch_1ms_cnt = 0;

void sch_init(void) {
    sch_1ms_flag = 0;
    sch_1ms_cnt = 0;
}

void sch_trigger(void) {
    sch_1ms_flag = 1;
}

void sch_enter(void) {
    while (1) {
        if (sch_1ms_flag) {
            sch_1ms_flag = 0;
            sch_1ms_cnt++;
            sch_task1ms();
        }
        if (sch_1ms_cnt >= 10) {
            sch_1ms_cnt = 0;
            sch_task10ms();
        }
    }
}
