#include "bsp/osh_phy.h"
#include "hal/evsys.h"
#include "bsp/sensor.h"
#include "bsp/config.h"

#include "atsamd21e18a.h"

static uint32_t accumulates[OSH_CHANNEL_COUNT];

void osh_phy_init(void) {
    osh_phy_impl_init();

    // configure TC3 and TC4
    TC3_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1 ;
    TC3_REGS->COUNT16.TC_CTRLC = TC_CTRLC_CPTEN0_Msk | TC_CTRLC_CPTEN1_Msk;
    TC3_REGS->COUNT16.TC_EVCTRL = TC_EVCTRL_EVACT_PPW | TC_EVCTRL_TCEI_Msk;
    TC3_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk | TC_INTENSET_ERR_Msk | TC_INTENSET_MC1_Msk;

    NVIC_SetPriority(TC3_IRQn, 3);
    NVIC_EnableIRQ(TC3_IRQn);
    EVSYS_ConfigureUser(EVENT_ID_USER_TC3_EVU, osh_phy_get_evgen(0));

    TC4_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1 ;
    TC4_REGS->COUNT16.TC_CTRLC = TC_CTRLC_CPTEN0_Msk | TC_CTRLC_CPTEN1_Msk;
    TC4_REGS->COUNT16.TC_EVCTRL = TC_EVCTRL_EVACT_PPW | TC_EVCTRL_TCEI_Msk;
    TC4_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk | TC_INTENSET_ERR_Msk | TC_INTENSET_MC1_Msk; // it was MC0 but that was not handled??

    NVIC_SetPriority(TC4_IRQn, 3);
    NVIC_EnableIRQ(TC4_IRQn);

    // TC4 connected to channel 1
    EVSYS_ConfigureUser(EVENT_ID_USER_TC4_EVU, osh_phy_get_evgen(1));


    // TC5_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1 ;
    // TC5_REGS->COUNT16.TC_CTRLC = TC_CTRLC_CPTEN0_Msk | TC_CTRLC_CPTEN1_Msk;
    // TC5_REGS->COUNT16.TC_EVCTRL = TC_EVCTRL_EVACT_PPW | TC_EVCTRL_TCEI_Msk;
    // TC5_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk | TC_INTENSET_ERR_Msk | TC_INTENSET_MC1_Msk; // it was MC0 but that was not handled??

    // NVIC_SetPriority(TC5_IRQn, 3);
    // NVIC_EnableIRQ(TC5_IRQn);

    // // TC4 connected to channel 1
    // EVSYS_ConfigureUser(0x14, osh_phy_get_evgen(2));
}

void osh_phy_turn_on(void) {
    osh_phy_impl_turn_on();

    TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    TC4_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
}

void osh_phy_turn_off(void) {
    TC3_REGS->COUNT16.TC_CTRLA &= ~(TC_CTRLA_ENABLE_Msk);
    TC4_REGS->COUNT16.TC_CTRLA &= ~(TC_CTRLA_ENABLE_Msk);

    // TODO: wait for stop

    osh_phy_impl_turn_off();
}

void TC3_Handler(void) {
    if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
        // TODO: notify the sensor about the error
    }
    else if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        // TODO: accumulate the impulses up to the sensor limits
    }
    else if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        uint16_t cc0 = TC3_REGS->COUNT16.TC_CC[0];
        uint16_t cc1 = TC3_REGS->COUNT16.TC_CC[1];
        osh_sensor_process(0, cc0, cc1);
    }

    TC3_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}

void TC4_Handler(void) {
    if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
        // TODO: notify the sensor about the error
    }
    else if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        // TODO: accumulate the impulses up to the sensor limits
    }
    else if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        uint16_t cc0 = TC4_REGS->COUNT16.TC_CC[0];
        uint16_t cc1 = TC4_REGS->COUNT16.TC_CC[1];
        osh_sensor_process(1, cc0, cc1);
    }

    TC4_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}

void TC5_Handler(void) {
    if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
        // TODO: notify the sensor about the error
    }
    else if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        // TODO: accumulate the impulses up to the sensor limits
    }
    else if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        uint16_t cc0 = TC5_REGS->COUNT16.TC_CC[0];
        uint16_t cc1 = TC5_REGS->COUNT16.TC_CC[1];
        osh_sensor_process(1, cc0, cc1);
    }

    TC5_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}
