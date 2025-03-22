#if !defined(APP_GEAR_H_)
#define APP_GEAR_H_

#include <stdint.h>
#include "bsp/sensor.h"

typedef enum {
    GEAR_Status_Unknown,
    GEAR_Status_Ok,
    GEAR_Status_OutOfRange,
    GEAR_Status_Error
} GEAR_Status_t;

/**
 * @brief Initializes the gear estimation service
 */
void GEAR_Initialize(void);

/**
 * @brief Updates the estimated gear
 */
void GEAR_Update(void);

/**
 * @brief Called when new speed data is available
 * 
 * @param channel Speed sensor channel
 * @param sample Speed sensor data
 */
void GEAR_OnTick(uint8_t channel, osh_sensor_sample_t sample);

/**
 * @brief Returns the current status of the gear estimation
 * 
 * @return GEAR_Status_t 
 */
GEAR_Status_t GEAR_GetStatus(void);

/**
 * @brief Returns the current estimated gear
 * 
 * @return uint8_t Estimated gear
 */
uint8_t GEAR_GetGear(void);

#endif // APP_GEAR_H_
