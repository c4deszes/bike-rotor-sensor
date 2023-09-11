#include "bsp/sensor.h"
#include "bsp/config.h"

osh_sensor_state states[OSH_CHANNEL_COUNT];

void osh_sensor_process(uint8_t channel, uint32_t width, uint32_t period) {
    uint16_t speed = (100u * 10000000u) / period;
    // TODO: validate pulse width and period
    // TODO: clamp speed down at large pulse periods

    bool op_result = osh_sensor_put_data(channel, speed);
    if (op_result == false) {
        states[channel] = osh_sensor_state_error;
    }
    else {
        states[channel] = osh_sensor_state_ok;
    }
}

osh_sensor_direction osh_sensor_get_direction(uint8_t channel) {
    return osh_sensor_direction_unknown;
}

osh_sensor_state osh_sensor_get_state(uint8_t channel) {
    return states[channel];
}
