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
 */
void osh_init(void);

/**
 * @brief Turns on the output stage
 */
void osh_output_on(void);

/**
 * @brief Turns off the output stage
 */
void osh_output_off(void);
