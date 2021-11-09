#pragma once

#include <stdint.h>

typedef struct {
    uint8_t boot_count;
} boot_state_t;

extern boot_state_t boot_state __attribute__((section(".shared_data")));

/**
 * @brief Returns the number 
 * 
 * @return uint8_t 
 */
inline uint8_t boot_count(void) {
    return boot_state.boot_count;
}

/**
 * @brief Resets the boot count
 */
inline void boot_ok(void) {
    boot_state.boot_count = 0;
}
