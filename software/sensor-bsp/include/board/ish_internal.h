#pragma once

#include <stdint.h>

/**
 * @brief Sets up the Input stage handler's input and output
 */
void ish_setup_io(void);

/**
 * @brief Returns the event channel that the Input stage will trigger
 * 
 * @return uint8_t Event channel number
 */
uint8_t ish_get_event_channel(void);
