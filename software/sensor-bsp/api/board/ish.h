/**
 * @file ish.h
 * @author Balazs Eszes
 * @brief Input Stage Handler
 * 
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef void (*ish_callback)(const uint16_t width);

/**
 * @brief Initializes the input stage driver with the given configuration
 * 
 * @param configuration 
 */
void ish_init(ish_callback callback);

/**
 * @brief Enables the input stage
 * 
 */
void ish_enable(void);

/**
 * @brief Disables the input stage
 */
void ish_disable(void);

/**
 * @brief Returns the event channel that the Input stage will trigger
 * 
 * @return uint8_t Event channel number
 */
uint8_t ish_get_event_channel(void);

/**
 * @brief Returns the immediate state of the input stage
 * 
 * @return true 
 * @return false 
 */
bool ish_get_state(void);
