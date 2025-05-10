#include "app/gear.h"

#include "app/feature.h"
#include "app/config.h"
#include "app/cadence.h"
#include "app/sec.h"
#include "common/swtimer.h"

/* Drivetrain status */
static uint8_t GEAR_CurrentGear;
static GEAR_Status_t GEAR_Status;

static uint16_t GEAR_HoldTimerCnt;
static uint16_t GEAR_FrontWheel_RotatePeriod;
static uint16_t GEAR_RearWheel_RotatePeriod;
static uint16_t GEAR_Crank_RotatePeriod;

void GEAR_Initialize(void) {
    GEAR_CurrentGear = 0;
    GEAR_Status = GEAR_Status_Unknown;
    GEAR_HoldTimerCnt = 0;
}

/**
 * @brief Returns the current gear based on the crank and wheel rotation periods
 * 
 * If estimated rear teeth are less than the minimum teeth of the cassette, the gear is 0.
 * If estimated rear teeth are more than the maximum teeth of the cassette, the gear is the maximum gear + 1.
 * If estimated rear teeth are within the cassette range then it returns with the index of that gear, starting from 1.
 * 
 * @param crank_period 
 * @param wheel_period 
 * @return uint8_t 
 */
static uint8_t GEAR_CalculateGear(uint16_t crank_period, uint16_t wheel_period) {
    uint8_t teeth = (CONFIG_Props.Gear_ChainringTeeth * wheel_period) / crank_period;

    if (teeth < GEAR_CASSETTE_MIN_TEETH) {
        return 0;
    }
    else if (teeth > GEAR_CASSETTE_MAX_TEETH) {
        return CONFIG_Props.Gear_MaxGears + 1;
    }

    // TODO: change this to find the closest gear
    for (uint8_t i = 0; i < CONFIG_Props.Gear_MaxGears; i++) {
        if (teeth <= CONFIG_Props.Gear_CassetteTeeth[i]) {
            return i + 1;
        }
    }

    return CONFIG_Props.Gear_MaxGears + 1;
}

void GEAR_Update(void) {
    if (SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) == sec_state_off) {
        GEAR_Status = GEAR_Status_Unknown;
        GEAR_CurrentGear = 0;
    }
    else if (SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_ok) {
        GEAR_Status = GEAR_Status_Error;
        GEAR_CurrentGear = 0;
    }
    else {
        if (SEC_GetChannelState(SPM_REAR_SENSOR_CHANNEL) == sec_state_ok) {
            uint8_t calculated_gear = GEAR_CalculateGear(GEAR_Crank_RotatePeriod, GEAR_RearWheel_RotatePeriod);

            if (GEAR_CurrentGear != calculated_gear && GEAR_HoldTimerCnt < CONFIG_Props.Gear_HoldTime) {
                GEAR_HoldTimerCnt += 100u;      // TODO: somewhere specify the tick time
            }
            else if(GEAR_CurrentGear != calculated_gear) {
                GEAR_CurrentGear = calculated_gear;
                GEAR_HoldTimerCnt = 0;
                GEAR_Status = GEAR_Status_Ok;
            }
            else {
                GEAR_HoldTimerCnt = 0;
            }
        }
        else if (SEC_GetChannelState(SPM_FRONT_SENSOR_CHANNEL) == sec_state_ok && CAD_GetStatus() == CADENCE_Status_Ok) {
            uint8_t calculated_gear = GEAR_CalculateGear(GEAR_Crank_RotatePeriod, GEAR_FrontWheel_RotatePeriod);

            if (GEAR_CurrentGear != calculated_gear && GEAR_HoldTimerCnt < CONFIG_Props.Gear_HoldTime) {
                GEAR_HoldTimerCnt += 100u;      // TODO: somewhere specify the tick time
            }
            else if(GEAR_CurrentGear != calculated_gear) {
                GEAR_CurrentGear = calculated_gear;
                GEAR_HoldTimerCnt = 0;
                GEAR_Status = GEAR_Status_Ok;
            }
            else {
                GEAR_HoldTimerCnt = 0;
            }
        }
        else if (CAD_GetStatus() == CADENCE_Status_Coasting) {
            // TODO: or maybe do nothing instead of setting to unknown
            GEAR_Status = GEAR_Status_Unknown;
        }
        else {
            GEAR_Status = GEAR_Status_Error;
            GEAR_CurrentGear = 0;
        }
    }
}

void GEAR_OnTick(uint8_t channel, osh_sensor_sample_t sample) {
    switch (channel) {
        case SPM_FRONT_SENSOR_CHANNEL:
            GEAR_FrontWheel_RotatePeriod = sample.period * CONFIG_Props.FrontWheel_PoleCount;
            break;
        case SPM_REAR_SENSOR_CHANNEL:
            GEAR_RearWheel_RotatePeriod = sample.period * CONFIG_Props.RearWheel_PoleCount;
            break;
        case SPM_CRANK_SENSOR_CHANNEL:
            GEAR_Crank_RotatePeriod = sample.period * CONFIG_Props.CrankArm_PoleCount;
            break;
        default:
            break;
    }
}

GEAR_Status_t GEAR_GetStatus(void) {
    return GEAR_Status;
}

uint8_t GEAR_GetGear(void) {
    return GEAR_CurrentGear;
}
