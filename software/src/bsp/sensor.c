#include "bsp/sensor.h"
#include "bsp/config.h"

#include "bsp/sensor_internal.h"

// uint16_t osh_sensor_data[OSH_CHANNEL_COUNT][OSH_SENSOR_BUFFER_SIZE];
// uint8_t tail[OSH_CHANNEL_COUNT];
// uint8_t head[OSH_CHANNEL_COUNT];

osh_sensor_data sensors[OSH_CHANNEL_COUNT];

const osh_sensor_sample_t error_sample = {.pos = 0, .neg = 0};

void SENSOR_Process(uint8_t channel, uint32_t width, uint32_t period) {
    osh_sensor_sample_t sample = {.pos = width, .neg = period - width};

    // TODO: support for other sensors (e.g.: TLE4942)
    GENERIC_SENSOR_Process(channel, sample);
}

osh_sensor_sample_t SENSOR_GetSample(uint8_t channel) {
    if(channel >= OSH_CHANNEL_COUNT) {
        return error_sample;
    }
    if (sensors[channel].head == sensors[channel].tail) {
        return error_sample;
    }
    osh_sensor_sample_t entry = sensors[channel].samples[sensors[channel].tail];
    sensors[channel].tail = (sensors[channel].tail + 1) % 64;
    return entry;
}

uint8_t SENSOR_HasData(uint8_t channel) {
    if(channel >= OSH_CHANNEL_COUNT) {
        return 0;
    }
    return ((uint8_t)(OSH_SENSOR_BUFFER_SIZE + sensors[channel].head - sensors[channel].tail)) % OSH_SENSOR_BUFFER_SIZE;
}

bool SENSOR_PutData(uint8_t channel, osh_sensor_sample_t sample) {
    if(channel >= OSH_CHANNEL_COUNT) {
        return false;
    }
    uint8_t next = (sensors[channel].head + 1) % OSH_SENSOR_BUFFER_SIZE;
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
