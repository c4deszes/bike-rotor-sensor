#include "sensor/spe.h"

/** Configuration */
static uint8_t pulse_per_rotation;

/** State */
static uint16_t speed = 0;
static spe_speed_state state = SPE_SPEED_STATE_UNKNOWN;

void spe_init(const spe_configuration* conf) {
    pulse_per_rotation = conf->pulse_per_rotation;

    speed = 0;
    state = SPE_SPEED_STATE_UNKNOWN;
}

void spe_update(uint8_t pos, uint8_t neg) {
    if (pos == 0) {
        speed = 0;
        state = SPE_SPEED_STATE_OK;
    }
    else {
        /**
         * Positive Impulse length = 50ms
         * Negative Impulse length = 50ms
         * Pulse per rotation = 8
         * -> Total rotation time = (pos + neg) * pulse_per_rot
         * -> RPM = 60 * 1000 / total_rotation_time
         */
        uint16_t current_speed = (60 * 1000) / (pos + neg) * pulse_per_rotation;

        speed = current_speed;
        state = SPE_SPEED_STATE_OK;
    }
}

uint16_t spe_get_speed(void) {
    return speed;
}

spe_speed_state spe_get_speed_state(void) {
    return state;
}
