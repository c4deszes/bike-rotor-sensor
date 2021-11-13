#pragma once

#include <stdbool.h>

typedef enum {
    CRCSCAN_SOURCE_FLASH = 0x00,
    CRCSCAN_SOURCE_BOOTAPP = 0x01,
    CRCSCAN_SOURCE_BOOT = 0x02
} crcscan_source;

/**
 * @brief Enables the CRCSCAN peripheral, which will immediately triggers a CRC scan.
 * The user has to ensure that source setting has been set beforehand.
 */
void crcscan_enable(void);

/**
 * @brief Resets the CRCSCAN peripheral.
 * 
 * @note If peripheral's interrupt is enabled then the reset has to be performed
 * when the peripheral is not busy.
 */
void crcscan_reset(void);

/**
 * @brief Enables the Non-maskable interrupt (NMI) for the peripheral, which will be
 * triggered when there's a CRC failure.
 * 
 * @note Interrupt can only be enabled when the peripheral is not busy and afterwards
 * it cannot be cleared until a reset.
 */
void crcscan_enable_interrupt(void);

/**
 * @brief Sets the source region for the CRCSCAN
 * 
 * @param source Memory region to perform the scan on
 */
void crcscan_set_source(const crcscan_source source);

/**
 * @brief Returns the last CRC scan's result
 * 
 * @return true when the last scan was successful
 * @return false when the last scan was unsuccessful
 */
bool crcscan_is_ok(void);

/**
 * @brief Returns whether the CRCSCAN peripheral is busy at the moment
 * 
 * @return true If the peripheral is busy
 * @return false If the peripheral is not busy
 */
bool crcscan_is_busy(void);
