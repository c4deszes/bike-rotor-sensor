/**
 * @file current.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-15
 * 
 * @copyright Copyright (c) 2025
 * 
 * The current module is responsible for estimating the current consumed by device. It is based on
 * the sensor status and the input voltage.
 * 
 * Part of the current drawn is by the microcontroller, this is a DC/DC converter and so the current
 * depends on the input voltage.
 * 
 * The other part of the current draw is by the sensors, it depends on how many sensors are connected.
 */
#if !defined(__APP_CURRENT_H__)
#define __APP_CURRENT_H__

#include <stdint.h>

/**
 * @brief Initializes the current estimation service
 */
void CURRENT_Initialize(void);

/**
 * @brief Updates the estimated current
 */
void CURRENT_Update(void);

/**
 * @brief Returns the estimated current
 * 
 * @return The estimated current in mA
 */
uint16_t CURRENT_GetCurrent(void);

#endif // __APP_CURRENT_H__
