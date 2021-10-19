#include "sensor/spe.h"

uint16_t speed = 0;

void spe_init(void) {

}

void spe_update(uint8_t pos, uint8_t neg) {
    speed = neg;
}

uint16_t spe_get_speed(void) {
    return speed;
}

spe_speed_state spe_get_speed_state(void) {
    return SPE_SPEED_STATE_UNKNOWN;
}
