#include "sensor/pse.h"

#include <stdbool.h>

/** Configuration */
static uint8_t pulse_per_rotation;
static uint8_t index_threshold;

/** Internal state */
static uint8_t buffer[3];
static pse_position_state state = PSE_POSITION_STATE_UNKNOWN;
static uint8_t position_inc = 0; /** 0 - pulse_per_rotation */

void pse_init(sensor_configuration_t* conf) {
    pulse_per_rotation = conf->pulse_per_rotation;
    index_threshold = conf->index_threshold;

    /** Reset internal state */
    state = PSE_POSITION_STATE_UNKNOWN;
    position_inc = 0;

    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
}

void pse_shift_buffer(uint8_t width) {
    buffer[0] = buffer[1];
    buffer[1] = buffer[2];
    buffer[2] = width;
}

bool pse_find_index(void) {
    if (buffer[1] < buffer[0] + index_threshold) {
        return false;
    }
    if (buffer[1] < buffer[2] + index_threshold ) {
        return false;
    }
    return true;
}

void pse_zero(void) {
    state = PSE_POSITION_STATE_OK;
    position_inc = 0;
}

void pse_update(uint8_t pos, uint8_t neg) {
    if (pos == 0) {
        /** Ignore timeout */
        return;
    }

    // Shift in 
    pse_shift_buffer(pos);

    if (state == PSE_POSITION_STATE_UNKNOWN && pse_find_index()) {
        /** Synchronize */
        pse_zero();

        position_inc += 1;
    }
    else if (state == PSE_POSITION_STATE_OK) {
        /** Synchronized and pulse received */
        position_inc += 1;

        if (position_inc > pulse_per_rotation) {
            if (pse_find_index()) {
                pse_zero();
            }
            else {
                /** Late index */
                state = PSE_POSITION_STATE_UNKNOWN;
            }
        }
        else if(pse_find_index()) {
            /** Early index */
            state = PSE_POSITION_STATE_UNKNOWN;
        }
    }
}

uint16_t pse_get_position(void) {
    return position_inc;
}

pse_position_state pse_get_position_state(void) {
    return state;
}
