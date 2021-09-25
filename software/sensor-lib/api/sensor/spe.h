/**
 * @file spe.h
 * @author your name (you@domain.com)
 * @brief Speed Estimator is responsible for converting the 
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once

typedef enum {
    SPE_SPEED_STATE_UNKNOWN,
    SPE_SPEED_STATE_OK
} spe_speed_state;

void spe_init(void);

void spe_update(uint16_t width);

uint16_t spe_get_speed(void);

spe_speed_state spe_get_speed_state(void);
