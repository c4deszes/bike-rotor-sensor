#include "app/distance.h"
#include "app/config.h"
#include "app/sec.h"

/* Distance status variables */
static DIST_Status_t DIST_Status = DIST_Status_NotAvailable;
static uint32_t DIST_FrontDistance = 0;
static uint32_t DIST_RearDistance = 0;

static uint8_t DIST_FrontWheelPosition = 0;
static uint8_t DIST_RearWheelPosition = 0;

static uint16_t DIST_SensorErrorCounter = 0;

void DIST_Initialize(void) {
    DIST_ResetDistance();
}

static void DIST_CheckSensorError() {
    sec_state_t front_state = SEC_GetChannelState(SPM_FRONT_SENSOR_CHANNEL);
    sec_state_t rear_state = SEC_GetChannelState(SPM_REAR_SENSOR_CHANNEL);
    if ((front_state != sec_state_off && front_state != sec_state_ok) &&
        (rear_state != sec_state_off && rear_state != sec_state_ok) &&
        (DIST_SensorErrorCounter < DIST_MEASUREMENT_SENSOR_ERROR_TIMER_10MS)) {
        DIST_SensorErrorCounter++;
    }
    else if (DIST_SensorErrorCounter > 0) {
        DIST_SensorErrorCounter--;
    }

    if (DIST_SensorErrorCounter >= DIST_MEASUREMENT_SENSOR_ERROR_TIMER_10MS) {
        DIST_Status = DIST_Status_PermanentError;
    }
}

void DIST_Update(void) {
    if (DIST_Status == DIST_Status_NotAvailable) {
        // TODO: if ride begun only then set status to OK
        DIST_Status = DIST_Status_Ok;
    }
    else if (DIST_Status == DIST_Status_Ok) {
        DIST_CheckSensorError();

        if (DIST_FrontDistance - DIST_RearDistance > DIST_MEASUREMENT_DIFF_UP_THRESHOLD) {
            DIST_Status = DIST_Status_Error;
        }
    }
    else if (DIST_Status == DIST_Status_Error) {
        DIST_CheckSensorError();

        if (DIST_FrontDistance - DIST_RearDistance < DIST_MEASUREMENT_DIFF_DOWN_THRESHOLD) {
            DIST_Status = DIST_Status_Ok;
        }
    }
    else if (DIST_Status == DIST_Status_PermanentError) {
        // Do nothing
    }
    else {
        // Unsupported state
        DIST_Status = DIST_Status_PermanentError;
    }
}

// TODO: only count when ride is active??
void DIST_OnTick(uint8_t channel, osh_sensor_sample_t sample) {
    if (DIST_Status == DIST_Status_NotAvailable) {
        return;
    }

    if(sample.pos > 10) {
        if (channel == SPM_FRONT_SENSOR_CHANNEL || SEC_GetChannelState(SPM_FRONT_SENSOR_CHANNEL) != sec_state_ok) {
            DIST_FrontWheelPosition++;
        }

        if (channel == SPM_REAR_SENSOR_CHANNEL || SEC_GetChannelState(SPM_REAR_SENSOR_CHANNEL) != sec_state_ok) {
            DIST_RearWheelPosition++;
        }
    }

    if (DIST_FrontWheelPosition >= CONFIG_Props.FrontWheel_PoleCount) {
        DIST_FrontDistance += CONFIG_Props.FrontWheel_Circumference;
        DIST_FrontWheelPosition = 0;
    }
    if (DIST_RearWheelPosition >= CONFIG_Props.RearWheel_PoleCount) {
        DIST_RearDistance += CONFIG_Props.RearWheel_Circumference;
        DIST_RearWheelPosition = 0;
    }
}

void DIST_ResetDistance(void) {
    DIST_FrontDistance = 0;
    DIST_RearDistance = 0;
    DIST_FrontWheelPosition = 0;
    DIST_RearWheelPosition = 0;
    DIST_SensorErrorCounter = 0;
    DIST_Status = DIST_Status_NotAvailable;
}

uint32_t DIST_GetDistance() {
    return DIST_FrontDistance > DIST_RearDistance ? DIST_FrontDistance : DIST_RearDistance;
}

DIST_Status_t DIST_GetStatus(void) {
    return DIST_Status;
}
