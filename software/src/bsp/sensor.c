#include "bsp/sensor.h"
#include "app/config.h"
#include "bsp/sensor_internal.h"

osh_sensor_data sensors[3];

const osh_sensor_sample_t error_sample = {.pos = 0, .period = 0};

void SENSOR_Initialize(void) {
    for (uint8_t i = 0; i < 3; i++) {
        sensors[i].direction = osh_sensor_direction_unknown;
        sensors[i].state = osh_sensor_state_ok;
        sensors[i].head = 0;
        sensors[i].tail = 0;
    }
}

void SENSOR_Process(uint8_t channel, uint32_t width, uint32_t period) {
    osh_sensor_sample_t sample = {.pos = width, .period = period};

    // TODO: support for other sensors (e.g.: TLE4942)
    GENERIC_SENSOR_Process(channel, sample);
}

osh_sensor_sample_t SENSOR_GetSample(uint8_t channel) {
    if(channel >= 3) {
        return error_sample;
    }
    if (sensors[channel].head == sensors[channel].tail) {
        return error_sample;
    }
    osh_sensor_sample_t entry = sensors[channel].samples[sensors[channel].tail];
    sensors[channel].tail = (sensors[channel].tail + 1) % SPM_SENSOR_BUFFER_SIZE;
    return entry;
}

uint8_t SENSOR_HasData(uint8_t channel) {
    if(channel >= 3) {
        return 0;
    }
    return ((uint8_t)(SPM_SENSOR_BUFFER_SIZE + sensors[channel].head - sensors[channel].tail)) % SPM_SENSOR_BUFFER_SIZE;
}

bool SENSOR_PutData(uint8_t channel, osh_sensor_sample_t sample) {
    if(channel >= 3) {
        return false;
    }
    uint8_t next = (sensors[channel].head + 1) % SPM_SENSOR_BUFFER_SIZE;
    if (next == sensors[channel].tail) {
        return false;
    }
    sensors[channel].samples[sensors[channel].head] = sample;
    sensors[channel].head = next;
    return true;
}

osh_sensor_state_t SENSOR_GetState(uint8_t channel) {
    return sensors[channel].state;
}

void SENSOR_SetState(uint8_t channel, osh_sensor_state_t state) {
    sensors[channel].state = state;
}
