#include "app/itpms.h"
#include "app/speed.h"
#include "app/config.h"
#include "common/swtimer.h"

itpms_state_t itpms_state;
static bool itpms_startup;
static swtimer_t* itpms_startup_timer;

void ITPMS_Initialize(void) {
    itpms_startup = false;
    itpms_state = itpms_state_stopped;
    SWTIMER_Setup(itpms_startup_timer, ITPMS_STARTUP_TIME);
}

void ITPMS_Update(void) {

#if ITPMS_AUTO_STARTUP == 1
// TODO: only start when ride begins
    if (!itpms_startup && SWTIMER_Elapsed(itpms_startup_timer)) {
        itpms_startup = true;
        itpms_state = itpms_state_running;
    }
#endif

    if (itpms_state == itpms_state_running) {
        // TODO: implement
        // otherwise update the pressure diff. based on the speeds
        // if (spm_front_wheel.state != spm_speed_state_ok &&
        //     spm_front_wheel.state != spm_speed_slow_response &&
        //     spm_rear_wheel.state != spm_speed_state_ok &&
        //     spm_rear_wheel.state != spm_speed_slow_response) {
        //     itpms_state = itpms_state_stopped;
        // }
    }
}

void ITPMS_Start(void) {
    itpms_state = itpms_state_running;
}

void ITPMS_Stop(void) {
    itpms_state = itpms_state_stopped;
}
