/**
 * @file distance.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-12-07
 * 
 * @copyright Copyright (c) 2024
 * 
 * The distance module is responsible for calculating the distance traveled by the bicycle. It is
 * calculated by counting the number of wheel revolutions and multiplying it by the wheel circumference.
 * 
 * Front and rear wheel sensors are used to count the number of revolutions independently. The
 * distance is only accumulated once a ride starts.
 * 
 * The distance can be reset externally when the ride ends or when the user wants to reset it.
 * 
 * The module initially starts in the not available state. When the 
 * 
 */
#if !defined(APP_DISTANCE_H_)
#define APP_DISTANCE_H_

#include "bsp/sensor.h"

typedef enum {
    DIST_Status_NotAvailable,
    DIST_Status_Ok,
    DIST_Status_Error,
    DIST_Status_PermanentError
} DIST_Status_t;

/**
 * @brief Initializes the distance measurement service
 */
void DIST_Initialize(void);

/**
 * @brief Updates the distance measurement 
 */
void DIST_Update(void);

/**
 * @brief Callback function for the front and rear wheel sensors
 * 
 * @param channel The channel number of the sensor
 * @param sample The sensor sample
 */
void DIST_OnTick(uint8_t channel, osh_sensor_sample_t sample);

void DIST_ResetDistance(void);

uint32_t DIST_GetDistance();

DIST_Status_t DIST_GetStatus(void);

#endif // APP_DISTANCE_H_
