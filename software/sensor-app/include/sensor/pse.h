/**
 * @file pse.h
 * @author your name (you@domain.com)
 * @brief Position Estimator processes the impulses from optical slot sensors
 * and uses an index pulse to estimate the current angle of the rotor.
 * 
 * @details 
 */
#pragma once

#include <stdint.h>
#include "sensor/common.h"

typedef enum {
    PSE_POSITION_STATE_UNKNOWN,     /**< Signals that the current position is unknown */
    PSE_POSITION_STATE_OK           /**< Signals that the current position is valid */
} pse_position_state;

/**
 * @brief Initializes the Position Estimator with the given configuration
 * 
 * @param conf 
 */
void pse_init(sensor_configuration_t* conf);

/**
 * @brief Updates the position estimator's state
 * 
 */
void pse_update(uint16_t width, uint16_t period);

/**
 * @brief Returns the current position
 * 
 * @return uint16_t 
 */
uint16_t pse_get_position(void);

/**
 * @brief Returns the position estimator's state
 * 
 * @return pse_position_state 
 */
pse_position_state pse_get_position_state(void);
