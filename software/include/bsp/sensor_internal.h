#if !defined(BSP_SENSOR_INTERNAL_H_)
#define BSP_SENSOR_INTERNAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "bsp/sensor.h"

typedef struct {
    uint8_t tail;
    uint8_t head;
    osh_sensor_sample samples[64];  // TODO: move size to macro value
    osh_sensor_direction direction;
    osh_sensor_state state;
} osh_sensor_data;

/**
 * @brief Puts a sample onto the sample buffer
 * 
 * @param channel 
 * @param sample 
 * @return true 
 * @return false 
 */
bool SENSOR_PutData(uint8_t channel, osh_sensor_sample sample);

/**
 * @brief Sets the direction sensed by the sensor
 * 
 * @param channel Channel number
 * @param direction Rotational direction
 */
void SENSOR_SetDirection(uint8_t channel, osh_sensor_direction direction);

/**
 * @brief Sets the sensor state
 * 
 * @param channel Channel number
 * @param state Sensor state
 */
void SENSOR_SetState(uint8_t channel, osh_sensor_state state);

void GENERIC_SENSOR_Process(uint8_t channel, osh_sensor_sample sample);

#endif // BSP_SENSOR_INTERNAL_H_
