#if !defined(__APP_VOLT_H__)
#define __APP_VOLT_H__

#include <stdint.h>

/**
 * @brief Initializes the voltage measurement
 */
void VOLT_Initialize(void);

/**
 * @brief Updates the voltage measurement
 */
void VOLT_Update(void);

/**
 * @brief Returns the current voltage
 * 
 * @return uint16_t Voltage in millivolts
 */
uint16_t VOLT_GetVoltage(void);

#endif // __APP_VOLT_H__
