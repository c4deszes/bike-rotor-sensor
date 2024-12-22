#include "app/cadence.h"

#include "app/config.h"
#include "app/sec.h"

#include "common/swtimer.h"

static CADENCE_Status_t CAD_Status = CADENCE_Status_NotAvailable;

static uint32_t CAD_LastPeriod = 0;
static uint32_t CAD_PositionCounter = 0;
static uint8_t CAD_Position = 0;
static uint8_t CAD_Cadence = 0;

#define CAD_MINUTE_100US_REF 600000UL

void CAD_Initialize(void) {
    CAD_Status = CADENCE_Status_NotAvailable;
}

uint8_t CAD_CalculateCadence(uint32_t period) {
    if (period >= CAD_LOW_SPEED_CUTOFF_100US) {
        return (uint8_t)(CAD_MINUTE_100US_REF / CAD_LOW_SPEED_CUTOFF_100US);
    }
    else if(period <= CAD_HIGH_SPEED_CUTOFF_100US) {
        return (uint8_t)(CAD_MINUTE_100US_REF / CAD_HIGH_SPEED_CUTOFF_100US);
    }
    else {
        return (uint8_t)(CAD_MINUTE_100US_REF / period);
    }
}

// Currently sensor timeout is at 6.5s which means that we can measure down to ~10rpm
void CAD_Update(void) {
    if (CAD_Status == CADENCE_Status_NotAvailable) {
        // Transition to OK after the first revolution
        if (CAD_PositionCounter > 0) {
            CAD_Status = CADENCE_Status_Ok;
        }

        // Transition to error if the sensor is not ok
        if (SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_off &&
            SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_ok) {
            CAD_Status = CADENCE_Status_Error;
        }
    }
    else if (CAD_Status == CADENCE_Status_Ok || CAD_Status == CADENCE_Status_Coasting) {
        // Transition to error if the sensor is not ok
        if (SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_off &&
            SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_ok) {
            CAD_Status = CADENCE_Status_Error;
        }
        // Normal operation, calculate cadence, position and determine coasting
        else {
            if (CAD_PositionCounter > 1) {
                CAD_Cadence = CAD_CalculateCadence(CAD_LastPeriod);
                CAD_Status = CADENCE_Status_Ok;
                CAD_PositionCounter -= 1;

                //CAD_Position = (CAD_LastPeriod / 100 - CAD_PositionCounter) / (CAD_LastPeriod / 100 / CAD_POSITION_MAX);
            }
            else {
                // Timer elapsed
                if (CAD_LastPeriod < CAD_LOW_SPEED_CUTOFF_100US) {
                    CAD_LastPeriod += 100;
                    // TODO: function limits returned value to 10rpm
                    CAD_Cadence = CAD_CalculateCadence(CAD_LastPeriod);
                }
                CAD_Status = CADENCE_Status_Coasting;
                CAD_Position = CAD_POSITION_MAX;
            }
        }
    }
    else if (CAD_Status == CADENCE_Status_Error) {
        // Transition to normal mode if the sensor is ok
        // TODO: add a timer to prevent immediate transition
        if (SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) == sec_state_ok) {
            CAD_Status = CADENCE_Status_Coasting;
        }
    }
}

void CAD_OnTick(uint8_t channel, osh_sensor_sample_t sample) {
    if (channel == SPM_CRANK_SENSOR_CHANNEL) {
        // TODO: noise reject
        CAD_LastPeriod = sample.period;
        CAD_PositionCounter = (sample.period / 100) + CAD_COASTING_EXTRA_TIME; // TODO: extra time should be dependent on the cadence
        CAD_Position = 0;
    }
}

uint8_t CAD_GetPosition(void) {
    return CAD_Position;
}

uint8_t CAD_GetCadence(void) {
    return CAD_Cadence;
}

CADENCE_Status_t CAD_GetStatus(void) {
    return CAD_Status;
}
