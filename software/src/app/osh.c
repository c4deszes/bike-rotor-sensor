#include "app/osh.h"
#include "bsp/osh_phy.h"
#include "bsp/sensor.h"

static bool _enabled = false;
static bool _status = false;

osh_state _channels[4];

void _osh_set_all_channels(osh_state state) {

}

void osh_init(void) {
    osh_phy_init();

    _enabled = false;
    _status = false;
    _osh_set_all_channels(OSH_STATE_OFF);
}

void osh_update(void) {
    if(_enabled && !_status) {
        // Turn on PHY
        _osh_set_all_channels(OSH_STATE_OK);
        _status = true;
    }
    if(!_enabled && _status) {
        // Turn off PHY
        _osh_set_all_channels(OSH_STATE_OFF);
        _status = false;
    }

    if (_status) {
        osh_phy_update();

        
    }
}

void osh_turn_on(void) {
    _enabled = true;
}

void osh_turn_off(void) {
    _enabled = false;
}
