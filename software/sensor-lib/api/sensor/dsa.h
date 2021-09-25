/**
 * @file dsa.h
 * @author your name (you@domain.com)
 * @brief Distance Accumulator processes impulses and accumulates them into
 * 
 * 
 * 
 */
#pragma once

#include <stdint.h>

typedef struct {
    uint16_t min_pulse_width;
} dsa_configuration;

void dsa_init(const dsa_configuration* conf);

void dsa_update(uint16_t width);

uint32_t dsa_get_rotations(void);
