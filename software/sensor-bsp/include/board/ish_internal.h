#pragma once

#include <stdint.h>

/**
 * @brief Sets up the Input stage handler's input and output
 * 
 * 
 */
void ish_setup_io(void);

/**
 * @brief 
 * 
 * @return uint8_t 
 */
uint8_t ish_get_event_channel(void);
