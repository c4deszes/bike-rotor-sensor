#include "app/cadence.h"

#include "app/config.h"
#include "app/sec.h"

#include "common/swtimer.h"

static CADENCE_Status_t CAD_Status = CADENCE_Status_NotAvailable;

static uint32_t CAD_PositionCounter = 0;
static uint8_t CAD_Position = 0;
static uint8_t CAD_Cadence = 0;

#define CAD_MINUTE_US_REF 60000000UL

void CAD_Initialize(void) {
    CAD_Status = CADENCE_Status_NotAvailable;
}

void CAD_Update(void) {
    if (CAD_Status == CADENCE_Status_NotAvailable) {
        // Transition to OK after the first revolution
        if (CAD_PositionCounter > 0) {
            CAD_Status = CADENCE_Status_Ok;
        }
        if (SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_off &&
            SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_ok) {
            CAD_Status = CADENCE_Status_Error;
        }
    }
    else if (CAD_Status == CADENCE_Status_Ok || CAD_Status == CADENCE_Status_Coasting) {
        if (SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_off &&
            SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) != sec_state_ok) {
            CAD_Status = CADENCE_Status_Error;
        }

        if (CAD_PositionCounter > 9) {
            CAD_Status = CADENCE_Status_Ok;
            CAD_PositionCounter -= 10;  // 10 ms

            // TODO: increment as time elapses
            if (CAD_Position < CAD_POSITION_MAX - 1) {
                CAD_Position++;
            }
        }
        else {
            // Timer elapsed
            CAD_Position = CAD_POSITION_MAX;
            CAD_Status = CADENCE_Status_Coasting;
        }
    }
    else if (CAD_Status == CADENCE_Status_Error) {
        if (SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL) == sec_state_ok) {
            CAD_Status = CADENCE_Status_Ok;
        }
    }
}

void CAD_OnTick(uint8_t channel, osh_sensor_sample_t sample) {
    if (channel == SPM_CRANK_SENSOR_CHANNEL) {
        // TODO: noise reject
        // TODO: period reject

        // disabled as it would throw away coasting data
        // if (sample.pos <= CAD_NOISE_CUTOFF_DUTY_US) {
        //     return;
        // }

        if (sample.period >= CAD_LOW_SPEED_CUTOFF_US) {
            CAD_Cadence = CAD_MINUTE_US_REF / CAD_LOW_SPEED_CUTOFF_US;
            // TODO: in this case set state to coasting
        }
        else if(sample.period <= CAD_HIGH_SPEED_CUTOFF_US) {
            CAD_Cadence = CAD_MINUTE_US_REF / CAD_HIGH_SPEED_CUTOFF_US;
        }
        else {
            CAD_Cadence = (uint8_t)(CAD_MINUTE_US_REF / sample.period);
        }

        CAD_PositionCounter = sample.period / 1000;
        CAD_PositionCounter = CAD_PositionCounter + CAD_COASTING_EXTRA_TIME;
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
    return CADENCE_Status_NotAvailable;
}
