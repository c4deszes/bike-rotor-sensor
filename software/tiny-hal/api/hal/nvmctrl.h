#pragma once

#define USE_EEPROM __attribute__((section(".eeprom")))

/**
 * @brief Locks the BOOT sections from reading and instruction fetching
 */
void nvmctrl_bootlock(void);
