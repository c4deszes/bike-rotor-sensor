#include "sensor/spe.h"
#include "hal/sys.h"

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

void spe_update(uint16_t width, uint16_t period) {
    uint16_t current_speed = 0;
    if (period == 0) {
        speed = 0xFFFF;
        state = SPE_SPEED_STATE_UNKNOWN;
    }
    else {
        // TODO: width / period should be 30-70%
        if (width == 0) {
            current_speed = 0;
            state = SPE_SPEED_STATE_LOW;
        }
        else if (period < 100) {
            current_speed = sys_get_cpu_frequency() / 256u / period * 60u / pulse_per_rotation;
            state = SPE_SPEED_STATE_HIGH;
        }
        else if (period < 32768) {
            current_speed = sys_get_cpu_frequency() / 256u / period * 60u / pulse_per_rotation;
            speed = current_speed;
            state = SPE_SPEED_STATE_OK;
        }
        else {
            current_speed = 0;
            state = SPE_SPEED_STATE_LOW;
        }
        speed = (uint32_t)(speed + current_speed) / 2;
    }
}

uint16_t spe_get_speed(void) {
    return speed;
}

spe_speed_state spe_get_speed_state(void) {
    return state;
}
