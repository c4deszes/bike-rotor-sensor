#include "bsp/sensor.h"
#include "bsp/config.h"

uint16_t osh_sensor_data[OSH_CHANNEL_COUNT][OSH_SENSOR_BUFFER_SIZE];
uint8_t tail[OSH_CHANNEL_COUNT];
uint8_t head[OSH_CHANNEL_COUNT];

uint16_t osh_sensor_get_data(uint8_t channel) {
    if(channel >= OSH_CHANNEL_COUNT) {
        return 0xFFFF;
    }
    if (head[channel] == tail[channel]) {
        return 0xFFFF;
    }
    uint16_t entry = osh_sensor_data[channel][tail[channel]];
    tail[channel] = (tail[channel] + 1) % OSH_SENSOR_BUFFER_SIZE;
    return entry;
}

uint8_t osh_sensor_has_data(uint8_t channel) {
    if(channel >= OSH_CHANNEL_COUNT) {
        return 0;
    }
    return ((uint8_t)(OSH_SENSOR_BUFFER_SIZE + head[channel] - tail[channel])) % OSH_SENSOR_BUFFER_SIZE;
}

bool osh_sensor_put_data(uint8_t channel, uint16_t speed) {
    if(channel >= OSH_CHANNEL_COUNT) {
        return false;
    }
    uint8_t next = (head[channel] + 1) % OSH_SENSOR_BUFFER_SIZE;
    if (next == tail[channel]) {
        return false;
    }
    osh_sensor_data[channel][head[channel]] = speed;
    head[channel] = next;
    return true;
}
