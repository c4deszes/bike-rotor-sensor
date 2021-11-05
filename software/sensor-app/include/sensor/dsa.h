/**
 * @file dsa.h
 * @author your name (you@domain.com)
 * @brief Distance Accumulator processes impulses and accumulates them into
 * the number of accumulated rotations
 */
#pragma once

#include <stdint.h>

typedef struct {
    uint8_t pulse_per_rotation;
} dsa_configuration;

/**
 * @brief Initializes the Distance Accumulator with the given settings
 * 
 * @param conf DSA settings
 */
void dsa_init(const dsa_configuration* conf);

/**
 * @brief Updates the position signal by 
 * @note
 * 
 * @param pos Positive pulse length in milliseconds
 * @param neg Negative pulse length in milliseconds
 */
void dsa_update(uint8_t pos, uint8_t neg);

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
