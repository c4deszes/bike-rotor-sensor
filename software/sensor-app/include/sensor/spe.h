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

typedef enum {
    SPE_SPEED_STATE_UNKNOWN,
    SPE_SPEED_STATE_OK
} spe_speed_state;

typedef struct {
    uint8_t pulse_per_rotation;
} spe_configuration;

/**
 * @brief Initializes the Speed Estimator with the given configuration
 * 
 * @param conf 
 */
void spe_init(const spe_configuration* conf);

/**
 * @brief Updates the speed estimator's state
 * 
 * @param pos 
 * @param neg 
 */
void spe_update(uint8_t pos, uint8_t neg);

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
