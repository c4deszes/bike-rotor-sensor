#include "app/ride.h"

#include "app/speed.h"
#include "app/config.h"

static RIDE_Status_t RIDE_Status;
static uint16_t RIDE_Duration;
static uint32_t RIDE_SpeedSum;
static uint16_t RIDE_AverageSpeed;
static uint16_t RIDE_TopSpeed;
static uint16_t RIDE_TopSpeedWindow[RIDE_MONITOR_TOP_SPEED_WINDOW];

static uint8_t RIDE_IdleTimeoutCnt = 0;
static uint8_t RIDE_PauseTimeoutCnt = 0;

void RIDE_Initialize(void) {
    RIDE_Status = RIDE_Status_NotStarted;
    RIDE_Duration = 0;
    RIDE_SpeedSum = 0;
    RIDE_AverageSpeed = 0;
}

static void RIDE_UpdateTopSpeed(uint16_t last_speed) {
    // Shift the window and calculate the size of the window
    uint16_t window_top = RIDE_TopSpeedWindow[0];
    uint16_t window_bottom = RIDE_TopSpeedWindow[0];
    for (uint8_t i = 0; i < RIDE_MONITOR_TOP_SPEED_WINDOW; i++) {
        if (i < RIDE_MONITOR_TOP_SPEED_WINDOW - 1) {
            RIDE_TopSpeedWindow[i] = RIDE_TopSpeedWindow[i + 1];
        }
        else {
            RIDE_TopSpeedWindow[RIDE_MONITOR_TOP_SPEED_WINDOW - 1] = last_speed;
        }

        if (RIDE_TopSpeedWindow[i] > window_top) {
            window_top = RIDE_TopSpeedWindow[i];
        }
        if (RIDE_TopSpeedWindow[i] < window_bottom) {
            window_bottom = RIDE_TopSpeedWindow[i];
        }
    }

    // Update the top speed if the window is within the delta and the top speed is higher
    if (window_top - window_bottom <= RIDE_MONITOR_TOP_SPEED_DELTA && window_top > RIDE_TopSpeed) {
        RIDE_TopSpeed = window_top;
    }
}

static void RIDE_UpdateAverageSpeed(uint16_t last_speed) {
    RIDE_SpeedSum += last_speed;
    RIDE_AverageSpeed = RIDE_SpeedSum / RIDE_Duration;
}

void RIDE_Update(void) {
    // Detect when ride begins
    uint16_t speed = SPEED_GetSpeed();

#if RIDE_MONITOR_AUTOSTART_ENABLED == 1

    // TODO: both wheel speeds above minimum
    // TODO: crank arm single revolution
    // TODO: if any of the sensors fail (or all) then the ride would start given the other
    //       conditions are met
    if (RIDE_Status == RIDE_Status_NotStarted &&
        speed >= RIDE_MONITOR_AUTOSTART_MINIMUM_SPEED &&
        (SPEED_GetStatus() == speed_status_ok || SPEED_GetStatus() == speed_status_slow_response)) {
        RIDE_Start();
    }

#endif

    if (RIDE_Status == RIDE_Status_Active || RIDE_Status == RIDE_Status_Idle) {
        if (RIDE_Duration < UINT16_MAX) {
            RIDE_Duration++;

            // Calculate average speed
            RIDE_UpdateAverageSpeed(speed);

            // Track maximum speed
            RIDE_UpdateTopSpeed(speed);

            // Track elevation

            // Track descent

            if ((SPEED_GetStatus() == speed_status_ok || SPEED_GetStatus() == speed_status_slow_response)) {
                if (speed < RIDE_MONITOR_IDLE_SPEED && RIDE_Status == RIDE_Status_Active) {
                    RIDE_IdleTimeoutCnt++;
                }
                else if (speed >= RIDE_MONITOR_IDLE_SPEED) {
                    RIDE_Status = RIDE_Status_Active;
                    RIDE_IdleTimeoutCnt = 0;
                }

                if (RIDE_IdleTimeoutCnt >= RIDE_MONITOR_IDLE_TIMEOUT_1S) {
                    RIDE_Status = RIDE_Status_Idle;
                }
            }

#if RIDE_MONITOR_AUTOPAUSE_ENABLED == 1
            if (RIDE_Status == RIDE_Status_Idle) {
                RIDE_PauseTimeoutCnt++;
                if (RIDE_PauseTimeoutCnt >= RIDE_MONITOR_PAUSE_TIMEOUT_1S) {
                    RIDE_Pause();
                }
            }
            else {
                RIDE_PauseTimeoutCnt = 0;
            }
#endif
        }
        else {
            RIDE_Pause();
        }
    }
    else if (RIDE_Status == RIDE_Status_Paused) {

#if RIDE_MONITOR_AUTOUNPAUSE_ENABLED == 1
        if (speed >= RIDE_MONITOR_IDLE_SPEED) {
            RIDE_PauseTimeoutCnt++;
            if (RIDE_PauseTimeoutCnt >= RIDE_MONITOR_UNPAUSE_TIMEOUT_1S) {
                RIDE_Resume();
            }
        }
        else {
            RIDE_PauseTimeoutCnt = 0;
        }
#endif
    }
}

void RIDE_Start(void) {
    if (RIDE_Status != RIDE_Status_NotStarted) {
        return;
    }

    RIDE_Status = RIDE_Status_Active;
    RIDE_Duration = 0;
    RIDE_AverageSpeed = 0;
    RIDE_SpeedSum = 0;
    RIDE_TopSpeed = 0;
    for (uint8_t i = 0; i < RIDE_MONITOR_TOP_SPEED_WINDOW; i++) {
        RIDE_TopSpeedWindow[i] = 0;
    }
}

void RIDE_Pause(void) {
    if (RIDE_Status == RIDE_Status_NotStarted) {
        return;
    }
    RIDE_Status = RIDE_Status_Paused;
}

void RIDE_Resume(void) {
    if (RIDE_Status != RIDE_Status_Paused) {
        return;
    }
    RIDE_IdleTimeoutCnt = 0;
    RIDE_Status = RIDE_Status_Active;
}

void RIDE_Stop(void) {
    if (RIDE_Status == RIDE_Status_NotStarted) {
        return;
    }
    RIDE_Status = RIDE_Status_NotStarted;
}

RIDE_Status_t RIDE_GetStatus(void) {
    return RIDE_Status;
}

uint16_t RIDE_GetAverageSpeed(void) {
    return RIDE_AverageSpeed;
}

uint16_t RIDE_GetTopSpeed(void) {
    return RIDE_TopSpeed;
}

uint16_t RIDE_GetDuration(void) {
    return RIDE_Duration;
}
