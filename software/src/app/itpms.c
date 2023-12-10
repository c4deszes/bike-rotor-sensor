#include "app/itpms.h"

itpms_state_t itpms_state = itpms_state_stopped;

// external config:
//  - channel mapping
//  - measurement duration/distance
//  - threshold

void ITPMS_Initialize(void) {
    itpms_state = itpms_state_running;
}

void ITPMS_Update(void) {
    if (itpms_state == itpms_state_running) {
        // TODO: implement
        // check speed and sensor states
        // if any error then stop the measurement (or discard the data)
        // otherwise update the pressure diff. based on the speeds
    }
}

void ITPMS_Start(void) {
    itpms_state = itpms_state_running;
}

void ITPMS_Stop(void) {
    itpms_state = itpms_state_stopped;
}