#include "common/swtimer.h"

#include <stdlib.h>

// TODO: maximum number of counters
static uint8_t handle_counter = 0;
static swtimer_t timers[32];

swtimer_t* SWTIMER_Create(void) {
    if (handle_counter >= 32) {
        return NULL;
    }
    return &timers[handle_counter++];
}

void SWTIMER_Setup(swtimer_t* timer, uint16_t millis) {
    timer->millis = millis;
}

void SWTIMER_Update1ms() {
    for (uint8_t i = 0; i < 32; i++) {
        if (timers[i].millis != 0) {
            timers[i].millis--;
        }
    }
}

bool SWTIMER_Elapsed(swtimer_t* timer) {
    return timer->millis == 0;
}
