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

void CAD_Initialize(void);

void CAD_Update(void);

void CAD_OnTick(uint8_t channel, osh_sensor_sample_t sample);

uint8_t CAD_GetPosition(void);

uint8_t CAD_GetCadence(void);

CADENCE_Status_t CAD_GetStatus(void);

#endif // APP_CADENCE_H_
