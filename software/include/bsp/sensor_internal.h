#if !defined(BSP_SENSOR_INTERNAL_H_)
#define BSP_SENSOR_INTERNAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "bsp/sensor.h"
#include "app/config.h"

typedef struct {
    uint8_t tail;
    uint8_t head;
    osh_sensor_sample_t samples[SPM_SENSOR_BUFFER_SIZE];
    osh_sensor_direction_t direction;
    osh_sensor_state_t state;
} osh_sensor_data;

/**
 * @brief Puts a sample onto the sample buffer
 * 
 * @param channel 
 * @param sample 
 * @return true 
 * @return false 
 */
bool SENSOR_PutData(uint8_t channel, osh_sensor_sample_t sample);

/**
 * @brief Sets the direction sensed by the sensor
 * 
 * @param channel Channel number
 * @param direction Rotational direction
 */
void SENSOR_SetDirection(uint8_t channel, osh_sensor_direction_t direction);

/**
 * @brief Sets the sensor state
 * 
 * @param channel Channel number
 * @param state Sensor state
 */
void SENSOR_SetState(uint8_t channel, osh_sensor_state_t state);

void GENERIC_SENSOR_Process(uint8_t channel, osh_sensor_sample_t sample);

#endif // BSP_SENSOR_INTERNAL_H_
