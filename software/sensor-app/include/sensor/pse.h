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

typedef struct {
    uint8_t pulse_per_rotation;
    uint8_t index_threshold;
} pse_configuration;

typedef enum {
    PSE_POSITION_STATE_UNKNOWN,
    PSE_POSITION_STATE_OK
} pse_position_state;

void pse_init(const pse_configuration* conf);

void pse_update(uint8_t pos, uint8_t neg);

uint16_t pse_get_position(void);

pse_position_state pse_get_position_state(void);
