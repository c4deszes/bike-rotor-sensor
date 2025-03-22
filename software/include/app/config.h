#if !defined(__APP_CONFIG_H__)
#define __APP_CONFIG_H__

#include "app/feature.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint16_t FrontWheel_Circumference;
    uint16_t RearWheel_Circumference;
    uint16_t CrankArm_Circumference;

    uint8_t FrontWheel_PoleCount;
    uint8_t RearWheel_PoleCount;
    uint8_t CrankArm_PoleCount;

    // TODO: sensor types

    uint16_t Gear_HoldTime;
    uint8_t Gear_MaxGears;
    uint8_t Gear_ChainringTeeth;
    uint8_t Gear_CassetteTeeth[GEAR_MAX_GEARS];

    bool Ride_AutoStart;
    bool Ride_AutoPause;
    bool Ride_AutoResume;
    uint16_t Ride_IdleSpeed;
    uint16_t Ride_IdleTimeout;
    uint16_t Ride_PauseTimeout;
    uint16_t Ride_ResumeTimeout;

    bool Altitude_Enabled;
    uint16_t Altitude_QNH;

    bool ITPMS_AutoStart;
    uint16_t ITPMS_MinimumSpeed;
    uint16_t ITPMS_SpeedThreshold;

} CONFIG_Properties_t;

extern CONFIG_Properties_t CONFIG_Props;

void CONFIG_LoadFlashProperties(void);

void CONFIG_ReloadUdsProperties(void);

#endif // __APP_CONFIG_H__
