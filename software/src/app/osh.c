#include "app/osh.h"
#include "bsp/osh_phy.h"
#include "bsp/sensor.h"

static bool _enabled = false;
static bool _status = false;

osh_state _channels[4];

void OSH_Initialize(void) {
    OSH_PhyInit();

    _enabled = false;
    _status = false;
    //_osh_set_all_channels(OSH_STATE_OFF);
}

void OSH_Update(void) {
    if(_enabled && !_status) {
        // Turn on PHY
        OSH_PhyTurnOn();
        //_osh_set_all_channels(OSH_STATE_OK);
        _status = true;
    }
    if(!_enabled && _status) {
        // Turn off PHY
        OSH_PhyTurnOff();
        //_osh_set_all_channels(OSH_STATE_OFF);
        _status = false;
    }
    OSH_PhyUpdate();
}

void OSH_TurnOn(void) {
    _enabled = true;
}

void OSH_TurnOff(void) {
    _enabled = false;
}
