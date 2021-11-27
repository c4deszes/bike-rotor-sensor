#pragma once

/**
 * @brief Initializes the bootloader
 */
void boot_init(void);

void boot_enter(void);

/**
 * @brief Starts the application
 */
void boot_startapp();
