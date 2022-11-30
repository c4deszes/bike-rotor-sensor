/**
 * @file spe.h
 * @author Balazs Eszes
 * @brief Speed Estimator processes the impulses from optical slot sensors and
 * calculates the angular velocity
 * 
 * @copyright Copyright (c) 2021
 */
#pragma once

#include <stdint.h>
#include "sensor/common.h"

typedef enum {
    SPE_SPEED_STATE_UNKNOWN,
    SPE_SPEED_STATE_LOW,
    SPE_SPEED_STATE_HIGH,
    SPE_SPEED_STATE_OK
} spe_speed_state;

/**
 * @brief Initializes the Speed Estimator with the given configuration
 * 
 * @param conf 
 */
void spe_init(sensor_configuration_t* conf);

/**
 * @brief Updates the speed estimator's state
 * 
 */
void spe_update(uint16_t width, uint16_t period);

/**
 * @brief Returns the current angular velocity in Rotations per minute (RPM)
 * 
 * @return uint16_t 
 */
uint16_t spe_get_speed(void);

/**
 * @brief Returns the speed estimator's state
 * 
 * @return spe_speed_state 
 */
spe_speed_state spe_get_speed_state(void);
