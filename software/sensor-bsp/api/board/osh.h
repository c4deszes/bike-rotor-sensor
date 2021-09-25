/**
 * @file osh.h
 * @author your name (you@domain.com)
 * @brief 
 * 
 */
#pragma once

#include <stdint.h>

/**
 * @brief Initializes the output stage driver
 * 
 */
void osh_init(void);

void osh_output_on(void);

void osh_output_off(void);

uint16_t osh_get_current(void);
