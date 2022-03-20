#include "sensor/spe.h"

/** Configuration */
static uint8_t pulse_per_rotation;

/** State */
static uint16_t speed = 0;
static spe_speed_state state = SPE_SPEED_STATE_UNKNOWN;

void spe_init(sensor_configuration_t* conf) {
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
        uint16_t current_speed = (60u * 1000u) / ((pos + neg) * pulse_per_rotation);

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
