#include "bsp/osh_phy.h"
#include "bsp/osh_phy_internal.h"
#include "hal/evsys.h"
#include "hal/tc.h"
#include "bsp/sensor.h"
#include "bsp/config.h"

#include "atsamd21e18a.h"

static uint32_t accumulates[OSH_CHANNEL_COUNT];
static osh_phy_channel_state_t phy_states[OSH_CHANNEL_COUNT];

void OSH_PhyInit(void) {
    OSH_PhyInit_Impl();

    TC_SetupCapture(TC3, tc_prescaler_div1);
    EVSYS_ConfigureUser(EVENT_ID_USER_TC3_EVU, OSH_PhyGetEventGenerator(0));

    TC_SetupCapture(TC4, tc_prescaler_div1);
    EVSYS_ConfigureUser(EVENT_ID_USER_TC4_EVU, OSH_PhyGetEventGenerator(1));
}

void OSH_PhySetChannelStatus(uint8_t channel, osh_phy_channel_state_t status) {
    phy_states[channel] = status;
}

osh_phy_channel_state_t OSH_PhyGetChannelState(uint8_t channel) {
    return phy_states[channel];
}

void OSH_PhyTurnOn(void) {
    OSH_PhyTurnOn_Impl();

    TC_Start(TC3);
    TC_Start(TC4);

    for (uint8_t i = 0; i < OSH_CHANNEL_COUNT; i++) {
        OSH_PhySetChannelStatus(i, osh_phy_channel_state_ok);
    }
}

void OSH_PhyTurnOff(void) {
    TC_Stop(TC3);
    TC_Stop(TC4);

    OSH_PhyTurnOff_Impl();

    for (uint8_t i = 0; i < OSH_CHANNEL_COUNT; i++) {
        OSH_PhySetChannelStatus(i, osh_phy_channel_state_off);
    }
}

// TODO: move interrupt handlers to HAL
void TC3_Handler(void) {
    if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
        // TODO: notify the sensor about the error
        accumulates[0] = 0;
    }
    else if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        // TODO: accumulate the impulses up to the sensor limits
        accumulates[0] += TC3_REGS->COUNT16.TC_COUNT; // or 0xFFFF
    }
    else if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        uint16_t cc0 = TC3_REGS->COUNT16.TC_CC[0];
        uint16_t cc1 = TC3_REGS->COUNT16.TC_CC[1];
        // TODO: question, to which te rm do we add the accumulate? was is stuck in high or low?
        SENSOR_Process(0, cc0, cc1);
        accumulates[0] = 0;
    }

    TC3_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}

// TODO: move interrupt handlers to HAL
void TC4_Handler(void) {
    if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
        // TODO: notify the sensor about the error, reset accumulates
        accumulates[1] = 0;
    }
    else if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        // TODO: accumulate the impulses up to the sensor limits
        accumulates[1] += TC4_REGS->COUNT16.TC_COUNT; // or 0xFFFF
    }
    else if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        uint16_t cc0 = TC4_REGS->COUNT16.TC_CC[0];
        uint16_t cc1 = TC4_REGS->COUNT16.TC_CC[1];
        accumulates[1] = 0;
        // TODO: question, to which term do we add the accumulate? was is stuck in high or low?
        SENSOR_Process(1, cc0, cc1);
    }

    TC4_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}

// void TC5_Handler(void) {
//     if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
//         // TODO: notify the sensor about the error
//     }
//     else if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
//         // TODO: accumulate the impulses up to the sensor limits
//     }
//     else if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
//         uint16_t cc0 = TC5_REGS->COUNT16.TC_CC[0];
//         uint16_t cc1 = TC5_REGS->COUNT16.TC_CC[1];
//         SENSOR_Process(1, cc0, cc1);
//     }

//     TC5_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
// }
