#pragma once

#include <stdint.h>

/**
 * @brief Initializes the microcontroller with the given clock frequency
 * 
 * @param cpu_frequency CPU Frequency
 */
void sys_init(uint32_t cpu_frequency);

/**
 * @brief Returns the current CPU frequency
 * 
 * @return uint32_t CPU frequency in Hz (1/s)
 */
uint32_t sys_get_cpu_frequency(void);

/**
 * @brief Returns the microcontroller's revision
 * 
 * @return uint8_t 
 */
uint8_t sys_get_cpu_rev(void);

/**
 * @brief Enables interrupts globally
 */
void sys_enable_interrupts(void);

/**
 * @brief Disables interrupts globally
 */
void sys_disable_interrupts(void);
