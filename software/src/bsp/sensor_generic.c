#include "bsp/sensor.h"
#include "bsp/sensor_internal.h"

void GENERIC_SENSOR_Process(uint8_t channel, osh_sensor_sample_t sample) {
    bool op_result = SENSOR_PutData(channel, sample);
    if (op_result == false) {
        SENSOR_SetState(channel, osh_sensor_state_error);
    }
    else {
        SENSOR_SetState(channel, osh_sensor_state_ok);
    }
}
