#if !defined(_APP_RIDE_H)
#define _APP_RIDE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    RIDE_Status_NotStarted,
    RIDE_Status_Active,
    RIDE_Status_Idle,
    RIDE_Status_Paused
} RIDE_Status_t;

void RIDE_Initialize(void);

void RIDE_Update(void);

uint16_t RIDE_GetDuration(void);

uint16_t RIDE_GetAverageSpeed(void);

uint16_t RIDE_GetTopSpeed(void);

RIDE_Status_t RIDE_GetStatus(void);



void RIDE_Start(void);

void RIDE_Pause(void);

void RIDE_Resume(void);

void RIDE_Stop(void);

#endif // _APP_RIDE_H
