#if !defined(APP_CADENCE_H_)
#define APP_CADENCE_H_

#include <stdint.h>

#include "bsp/sensor.h"

typedef enum {
    CADENCE_Status_NotAvailable,
    CADENCE_Status_Ok,
    CADENCE_Status_Coasting,
    CADENCE_Status_Error
} CADENCE_Status_t;

/**
 * @brief Initializes the cadence sensor
 */
void CAD_Initialize(void);

/**
 * @brief Updates the cadence measurement
 */
void CAD_Update(void);

/**
 * @brief Called when new speed data is available
 * 
 * @param channel Channel number
 * @param sample Speed data
 */
void CAD_OnTick(uint8_t channel, osh_sensor_sample_t sample);

/**
 * @brief Returns the current position of the crank arm
 * 
 * @return uint8_t Crank arm position
 */
uint8_t CAD_GetPosition(void);

/**
 * @brief Returns the current cadence
 * 
 * @return uint8_t Cadence in RPM
 */
uint8_t CAD_GetCadence(void);

/**
 * @brief Returns the current status of the cadence sensor and measurement
 * 
 * @return CADENCE_Status_t 
 */
CADENCE_Status_t CAD_GetStatus(void);

#endif // APP_CADENCE_H_
