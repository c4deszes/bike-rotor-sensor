/**
 * @file ish.h
 * @author Balazs Eszes
 * @brief Input Stage Handler is responsible for configuring the input stage peripheral
 * as well as buffering the incoming pulse data.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 
 * 
 */
typedef struct {
    uint8_t positive;   /**< Positive pulse length in milliseconds */
    uint8_t negative;   /**< Negative pulse length in milliseconds */
} ish_data;

/**
 * @brief Initializes the input stage driver
 */
void ish_init(void);

/**
 * @brief Enables the input stage
 */
void ish_enable(void);

/**
 * @brief Disables the input stage
 */
void ish_disable(void);

/**
 * @brief Returns the immediate state of the input stage
 * 
 * @return true if the input is high
 * @return false if the input is low
 */
bool ish_get_state(void);

/**
 * @brief Returns the number of not processed pulses
 * 
 * @return uint8_t Number of pulses in the buffer
 */
uint8_t ish_available();

/**
 * @brief Returns a pulse received by the input stage handler, if there's nothing
 * in left in the buffer, it's going to return a pulse where both positive and negative
 * lengths are 0xFF.
 * 
 * @return ish_data Pulse information
 */
ish_data ish_get();
