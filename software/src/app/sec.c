#include "app/sec.h"

#include "app/config.h"
#include "bsp/config.h"
#include "bsp/osh_phy.h"
#include "bsp/sensor.h"

#include "common/swtimer.h"

static bool _enabled = false;
static bool _status = false;

static bool _startup = false;
static swtimer_t* startup_timer;

static sec_state_t channel_states[OSH_CHANNEL_COUNT];

void SEC_Initialize(void) {
    _enabled = false;
    _status = false;
    startup_timer = SWTIMER_Create();
    SWTIMER_Setup(startup_timer, SENSOR_STARTUP_TIME);
}

void SEC_TurnOn(void) {
    _enabled = true;
}

void SEC_TurnOff(void) {
    _enabled = false;
}

static sec_state_t SEC_MapChannelState(osh_phy_channel_state_t phy, osh_sensor_state_t sensor) {
    if (phy == osh_phy_channel_state_off) {
        return sec_state_off;
    }
    else if (phy == osh_phy_channel_state_short) {
        return sec_state_short;
    }
    else if (phy == osh_phy_channel_state_open) {
        return sec_state_open;
    }
    else {
        if (sensor == osh_sensor_state_error) {
            return sec_state_error;
        }
        else if (sensor == osh_sensor_state_warning) {
            return sec_state_warning;
        }
        else {
            return sec_state_ok;
        }
    }
}

void SEC_Update(void) {
    if (!_startup && SWTIMER_Elapsed(startup_timer)) {
        _startup = true;
        SEC_TurnOn();
    }

    if(_enabled && !_status) {
        // Turn on PHY
        OSH_PhyTurnOn();
        _status = true;
    }
    if(!_enabled && _status) {
        // Turn off PHY
        OSH_PhyTurnOff();
        _status = false;
    }

    OSH_PhyUpdate();

    for (uint8_t i = 0; i < OSH_CHANNEL_COUNT; i++) {
        osh_phy_channel_state_t phy_state = OSH_PhyGetChannelState(i);
        osh_sensor_state_t sensor_state = SENSOR_GetState(i);
        channel_states[i] = SEC_MapChannelState(phy_state, sensor_state);
    }
}

sec_state_t SEC_GetChannelState(uint8_t channel) {
    return channel_states[channel];
}
