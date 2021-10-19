#include "sensor/dsa.h"

/** Configuration */
static uint8_t pulse_per_rotations;

/** State */
static uint8_t steps;
static uint32_t rotations;

void dsa_init(const dsa_configuration* conf) {
    pulse_per_rotations = conf->pulse_per_rotations;
    steps = 0;
    rotations = 0;
}

void dsa_update(uint8_t pos, uint8_t neg) {
    if (pos == 0) {
        /** Ignore timeout */
    }
    else {
        /** Increment rotation counter */
        steps++;
        if (steps >= pulse_per_rotations) {
            rotations++;
            steps = 0;
        }
    }
}

void dsa_reset(void) {
    steps = 0;
    rotations = 0;
}

uint32_t dsa_get_rotations(void) {
    return rotations;
}
