#if !defined(_APP_DIAGNOSTICS_H)
#define _APP_DIAGNOSTICS_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initializes the diagnostics module
 */
void DIAG_Initialize(void);

/**
 * @brief Updates the diagnostic related variables
 */
void DIAG_Update(void);

/**
 * @brief Returns whether the device should enter bootloader mode
 * 
 * @return true
 * @return false
 */
bool DIAG_BootRequest(void);

/**
 * @brief Resets all diagnostic requests
 */
void DIAG_ResetAllRequests(void);

#endif // _APP_DIAGNOSTICS_H
