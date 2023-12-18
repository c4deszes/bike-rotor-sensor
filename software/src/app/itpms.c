#include "app/itpms.h"
#include "app/spm.h"
#include "app/config.h"
#include "common/swtimer.h"

itpms_state_t itpms_state;
static bool _startup;
static swtimer_t* startup_timer;

// external config:
//  - channel mapping
//  - measurement duration/distance
//  - threshold

void ITPMS_Initialize(void) {
    _startup = false;
    itpms_state = itpms_state_stopped;
    SWTIMER_Setup(startup_timer, ITPMS_STARTUP_TIME);
}

void ITPMS_Update(void) {
    // Automatic startup
    if (!_startup && SWTIMER_Elapsed(startup_timer)) {
        _startup = true;
        itpms_state = itpms_state_running;
    }

    if (itpms_state == itpms_state_running) {
        // TODO: implement
        // otherwise update the pressure diff. based on the speeds
        if (spm_front_wheel.state != spm_speed_state_ok &&
            spm_front_wheel.state != spm_speed_slow_response &&
            spm_rear_wheel.state != spm_speed_state_ok &&
            spm_rear_wheel.state != spm_speed_slow_response) {
            itpms_state = itpms_state_stopped;
        }
    }
}

void ITPMS_Start(void) {
    itpms_state = itpms_state_running;
}

void ITPMS_Stop(void) {
    itpms_state = itpms_state_stopped;
}