#include "app/ride.h"

#include "app/speed.h"
#include "app/config.h"

static RIDE_Status_t RIDE_Status;
static uint16_t RIDE_Duration;

void RIDE_Initialize(void) {
    RIDE_Status = RIDE_Status_NotStarted;
    RIDE_Duration = 0;
}

void RIDE_Update(void) {
    // Detect when ride begins

#if RIDE_MONITOR_AUTOSTART_ENABLED == 1

    // TODO: both wheel speeds above minimum
    // TODO: crank arm single revolution
    // TODO: if any of the sensors fail (or all) then the ride would start given the other
    //       conditions are met
    if (RIDE_Status == RIDE_Status_NotStarted &&
        SPEED_GetSpeed() >= RIDE_MONITOR_AUTOSTART_MINIMUM_SPEED) {
        RIDE_Start();
    }

#endif

    if (RIDE_Status == RIDE_Status_Active || RIDE_Status == RIDE_Status_Idle) {
        if (RIDE_Duration < 0xFFFFu) {
            RIDE_Duration++;
        }
        else {
            RIDE_Pause();
        }
    }

    // Calculate average speed

    // Track maximum speed

    // Track elevation

    // Track descent
}

void RIDE_Start(void) {
    if (RIDE_Status != RIDE_Status_NotStarted) {
        return;
    }

    RIDE_Status = RIDE_Status_Active;
    RIDE_Duration = 0;
}

void RIDE_Pause(void) {
    if (RIDE_Status == RIDE_Status_NotStarted) {
        return;
    }
    RIDE_Status = RIDE_Status_Paused;
}

RIDE_Status_t RIDE_GetStatus(void) {
    return RIDE_Status;
}

uint16_t RIDE_GetDuration(void) {
    return RIDE_Duration;
}
