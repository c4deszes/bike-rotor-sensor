#include "app/itpms.h"
#include "app/speed.h"
#include "app/feature.h"
#include "common/swtimer.h"

itpms_state_t itpms_state;
static swtimer_t* itpms_startup_timer;

void ITPMS_Initialize(void) {
    itpms_state = itpms_state_stopped;
}

void ITPMS_Update(void) {

}

void ITPMS_Start(void) {
    itpms_state = itpms_state_running;
}

void ITPMS_Stop(void) {
    itpms_state = itpms_state_stopped;
}
