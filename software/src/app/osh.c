#include "app/osh.h"
#include "bsp/osh_phy.h"
#include "bsp/osh_sensor.h"

void osh_init(void) {
    
}

void osh_update(void) {
    osh_phy_update();
}

// Turn on both channels (operational)
//void osh_turn_on(void);

// Turn off both channels (low current)
//void osh_turn_off(void);