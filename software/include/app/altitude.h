/**
 * @file alt.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-04
 * 
 * @copyright Copyright (c) 2024
 * 
 * Altitude component is responsible for reading the pressure sensor and calculating the altitude
 * based on the pressure.
 * 
 */
#if !defined(APP_ALT_H_)
#define APP_ALT_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ALT_Status_NotAvailable,
    ALT_Status_Ok,
    ALT_Status_Error,
    ALT_Status_PermanentError
} ALT_Status_t;

/**
 * @brief Initializes the altitude sensor
 */
void ALT_Initialize(void);

/**
 * @brief Polls the pressure sensor for new data and updates the altitude value
 */
void ALT_Update(void);

/**
 * @brief Returns the current status of the altitude sensor
 * 
 * @return ALT_Status_t 
 */
ALT_Status_t ALT_GetStatus(void);

/**
 * @brief Returns whether the altitude sensor has a pressure error
 * 
 * @return true 
 * @return false 
 */
bool ALT_HasPressureError(void);

/**
 * @brief Returns whether the altitude sensor has a temperature error
 * 
 * @return true 
 * @return false 
 */
bool ALT_HasTemperatureError(void);

/**
 * @brief Returns the current altitude (in meters)
 * 
 * @return int16_t Altitude above sea level in meters
 */
int16_t ALT_GetAltitude(void);

/**
 * @brief Returns the current pressure
 * 
 * @return uint32_t Pressure in Pascals
 */
uint32_t ALT_GetPressure(void);

/**
 * @brief Sets the reference QNH value (pressure at sea level)
 * 
 * @param qnh 
 */
void ALT_SetQNH(uint32_t qnh);

/**
 * @brief Returns the current temperature in Kelvins
 * 
 * @return uint16_t Temperature in Kelvins
 */
uint16_t ALT_GetTemperature(void); 

#endif // APP_ALT_H_
