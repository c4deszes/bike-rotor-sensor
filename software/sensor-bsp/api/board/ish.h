/**
 * @file ish.h
 * @author Balazs Eszes
 * @brief Input Stage Handler
 * 
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t positive;
    uint8_t negative;
} ish_data;

/**
 * @brief Initializes the input stage driver
 */
void ish_init(void);

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

/**
 * @brief Returns the number of not processed pulses
 * 
 * @return uint8_t 
 */
uint8_t ish_available();

/**
 * @brief Returns the 
 * 
 * @return ish_data 
 */
ish_data ish_get();
