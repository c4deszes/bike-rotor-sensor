#include "hal/tc.h"
#include "atsamd21e18a.h"

#include <stdlib.h>

static tc_registers_t* get_peripheral(uint8_t timer) {
    switch (timer) {
        case TC3: return TC3_REGS;
        case TC4: return TC4_REGS;
        case TC5: return TC5_REGS;
    }
    // TODO: is this good?
    return NULL;
}

static IRQn_Type get_interrupt(uint8_t timer) {
    switch (timer) {
        case TC3: return TC3_IRQn;
        case TC4: return TC4_IRQn;
        case TC5: return TC5_IRQn;
    }
    // TODO: is this good?
    return (IRQn_Type)(PERIPH_MAX_IRQn + 1);
}

void TC_SetupCapture(uint8_t timer, tc_prescaler prescaler) {
    tc_registers_t* peripheral = get_peripheral(timer);

    peripheral->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | prescaler;
    peripheral->COUNT16.TC_CTRLC = TC_CTRLC_CPTEN0_Msk | TC_CTRLC_CPTEN1_Msk;
    peripheral->COUNT16.TC_EVCTRL = TC_EVCTRL_EVACT_PPW | TC_EVCTRL_TCEI_Msk;
    peripheral->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk | TC_INTENSET_ERR_Msk | TC_INTENSET_MC0_Msk;  // it was MC0 but that was not handled??

    NVIC_SetPriority(get_interrupt(timer), 7);
    NVIC_EnableIRQ(get_interrupt(timer));
}

void TC_Start(uint8_t timer) {
    tc_registers_t* peripheral = get_peripheral(timer);
    
    peripheral->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;

    // TODO: wait for sync
}

void TC_Stop(uint8_t timer) {
    tc_registers_t* peripheral = get_peripheral(timer);

    peripheral->COUNT16.TC_CTRLA &= ~(TC_CTRLA_ENABLE_Msk);

    // TODO: wait for sync
}
