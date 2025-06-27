#include "app/ride.h"

#include "app/speed.h"
#include "app/distance.h"
#include "app/ridelogs.h"
#include "app/feature.h"
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

static void RIDE_ResetStatistics(void) {
    RIDE_Duration = 0;
    RIDE_SpeedSum = 0;
    RIDE_AverageSpeed = 0;
    RIDE_TopSpeed = 0;
    for (uint8_t i = 0; i < RIDE_MONITOR_TOP_SPEED_WINDOW; i++) {
        RIDE_TopSpeedWindow[i] = 0;
    }
}

void RIDE_Update(void) {
    // Detect when ride begins
    uint16_t speed = SPEED_GetSpeed();

    // TODO: both wheel speeds above minimum
    // TODO: crank arm single revolution
    // TODO: if any of the sensors fail (or all) then the ride would start given the other
    //       conditions are met
    if (RIDE_Status == RIDE_Status_NotStarted &&
        CONFIG_Props.Ride_AutoStart &&
        speed >= CONFIG_Props.Ride_IdleSpeed &&
        (SPEED_GetStatus() == speed_status_ok || SPEED_GetStatus() == speed_status_slow_response)) {
        RIDE_Start();
    }

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
                if (speed < CONFIG_Props.Ride_IdleSpeed && RIDE_Status == RIDE_Status_Active) {
                    RIDE_IdleTimeoutCnt++;
                }
                else if (speed >= CONFIG_Props.Ride_IdleSpeed) {
                    RIDE_Status = RIDE_Status_Active;
                    RIDE_IdleTimeoutCnt = 0;
                }

                if (RIDE_IdleTimeoutCnt >= CONFIG_Props.Ride_IdleTimeout) {
                    RIDE_Status = RIDE_Status_Idle;
                }
            }

            if (RIDE_Status == RIDE_Status_Idle && CONFIG_Props.Ride_AutoPause) {
                RIDE_PauseTimeoutCnt++;
                if (RIDE_PauseTimeoutCnt >= CONFIG_Props.Ride_PauseTimeout) {
                    RIDE_Pause();
                }
            }
            else {
                RIDE_PauseTimeoutCnt = 0;
            }
        }
        else {
            RIDE_Pause();
        }
    }
    else if (RIDE_Status == RIDE_Status_Paused) {

        if (speed >= CONFIG_Props.Ride_IdleSpeed && CONFIG_Props.Ride_AutoResume) {
            RIDE_PauseTimeoutCnt++;
            if (RIDE_PauseTimeoutCnt >= CONFIG_Props.Ride_ResumeTimeout) {
                RIDE_Start();
            }
        }
        else {
            RIDE_PauseTimeoutCnt = 0;
        }
    }
}

void RIDE_Start(void) {
    if (RIDE_Status = RIDE_Status_NotStarted) {
        RIDE_Status = RIDE_Status_Active;

        DIST_ResetDistance();
        RIDE_ResetStatistics();
        RIDELOGS_ResetActiveRecord();
    }
    else if (RIDE_Status == RIDE_Status_Paused) {
        RIDE_IdleTimeoutCnt = 0;
        RIDE_PauseTimeoutCnt = 0;
        RIDE_Status = RIDE_Status_Active;
    }
}

void RIDE_Pause(void) {
    if (RIDE_Status == RIDE_Status_Active || RIDE_Status == RIDE_Status_Idle) {
        RIDE_Status = RIDE_Status_Paused;
    }
}

void RIDE_Stop(void) {
    if (RIDE_Status == RIDE_Status_Active ||
        RIDE_Status == RIDE_Status_Idle ||
        RIDE_Status == RIDE_Status_Paused) {

        RIDELOGS_UpdateActiveRecord();
        RIDELOGS_PushActiveRecord();
        RIDE_Status = RIDE_Status_NotStarted;
    }
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
