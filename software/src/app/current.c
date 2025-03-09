#include "app/current.h"

#include "app/sec.h"
#include "app/volt.h"
#include "app/altitude.h"
#include "app/config.h"

#define CURRENT_SENSOR_HIGH_CURRENT 14
#define CURRENT_SENSOR_LOW_CURRENT 7
#define CURRENT_SENSOR_OFF_CURRENT 0

// Baseline current consumption is 3mA (12V -> 3V3 )
#define CURRENT_BASELINE 3

static uint16_t CURRENT_Estimate = 0;

void CURRENT_Initialize(void) {
    CURRENT_Estimate = 0;
}

static uint16_t CURRENT_BaselineCurrent(uint16_t voltage, int16_t temperature) {
    // TODO: baseline depends on the voltage and temperature (and clock, and enabled peripherals)
    return CURRENT_BASELINE;
}

static uint16_t CURRENT_SensorCurrent(sec_state_t state) {
    if (state == sec_state_ok || state == sec_state_warning) {
        return ((CURRENT_SENSOR_LOW_CURRENT + CURRENT_SENSOR_HIGH_CURRENT) / 2);
    }
    else if (state == sec_state_error || state == sec_state_open || state == sec_state_short || state == sec_state_off) {
        return CURRENT_SENSOR_OFF_CURRENT;
    }
    else {
        return CURRENT_SENSOR_OFF_CURRENT;
    }
}

void CURRENT_Update(void) {
    CURRENT_Estimate = CURRENT_BaselineCurrent(VOLT_GetVoltage(), ALT_GetTemperature())
                       + CURRENT_SensorCurrent(SEC_GetChannelState(SPM_FRONT_SENSOR_CHANNEL))
                       + CURRENT_SensorCurrent(SEC_GetChannelState(SPM_REAR_SENSOR_CHANNEL))
                       + CURRENT_SensorCurrent(SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL));
}

uint16_t CURRENT_GetCurrent(void) {
    return CURRENT_Estimate;
}
