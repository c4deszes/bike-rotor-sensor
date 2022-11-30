/**
 * @file dsa.h
 * @author your name (you@domain.com)
 * @brief Distance Accumulator processes impulses and accumulates them into
 * the number of accumulated rotations
 */
#pragma once

#include <stdint.h>
#include "sensor/common.h"

/**
 * @brief Initializes the Distance Accumulator with the given settings
 * 
 * @param conf DSA settings
 */
void dsa_init(sensor_configuration_t* conf);

/**
 * @brief Updates the position signal by 
 * @note
 */
void dsa_update(uint16_t width, uint16_t period);

/**
 * @brief Resets the state of the DSA, effectively resetting the number of rotations
 */
void dsa_reset(void);

/**
 * @brief Returns the number of rotations
 * 
 * @return uint32_t Rotations
 */
uint32_t dsa_get_rotations(void);
