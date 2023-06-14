#include "bsp/osh_phy.h"
#include "hal/evsys.h"

#include "atsamd21e18a.h"

uint16_t captureHigh1 = 0;
uint16_t captureLow1 = 0;

uint16_t captureHigh2 = 0;
uint16_t captureLow2 = 0;

void osh_phy_init(void) {
    osh_phy_init_internal();

    // configure TC3 and TC4
    TC3_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1 ;
    TC3_REGS->COUNT16.TC_CTRLC = TC_CTRLC_CPTEN0_Msk | TC_CTRLC_CPTEN1_Msk;
    TC3_REGS->COUNT16.TC_EVCTRL = TC_EVCTRL_EVACT_PPW | TC_EVCTRL_TCEI_Msk;
    TC3_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk | TC_INTENSET_ERR_Msk | TC_INTENSET_MC1_Msk | TC_INTENSET_MC0_Msk;

    TC4_REGS->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | TC_CTRLA_PRESCALER_DIV1 ;
    TC4_REGS->COUNT16.TC_CTRLC = TC_CTRLC_CPTEN0_Msk | TC_CTRLC_CPTEN1_Msk;
    TC4_REGS->COUNT16.TC_EVCTRL = TC_EVCTRL_EVACT_PPW | TC_EVCTRL_TCEI_Msk;
    TC4_REGS->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk | TC_INTENSET_ERR_Msk | TC_INTENSET_MC0_Msk;

    NVIC_SetPriority(TC3_IRQn, 3);
    NVIC_EnableIRQ(TC3_IRQn);
    NVIC_SetPriority(TC4_IRQn, 3);
    NVIC_EnableIRQ(TC4_IRQn);

    //NVIC_SetPriority(EIC_IRQn, 3);
    //NVIC_EnableIRQ(EIC_IRQn);

    // TC3 connected to channel 0
    EVSYS_ConfigureUser(0x12, 1);

    // TC4 connected to channel 1
    EVSYS_ConfigureUser(0x13, 2);
}

void TC3_CaptureInterruptHandler(void) {
    if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        captureHigh1 = 0xFFFFu;
        captureLow1 = 0x0000u;
    }
    if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        captureHigh1 = TC3_REGS->COUNT16.TC_CC[1];
        captureLow1 = TC3_REGS->COUNT16.TC_CC[0];
    }

    TC3_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}

void TC4_CaptureInterruptHandler(void) {
    if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        captureHigh2 = 0xFFFFu;
        captureLow2 = 0x0000u;
    }
    //if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC0_Msk) == TC_INTFLAG_MC0_Msk) {
    //    captureLow2 = TC4_REGS->COUNT16.TC_CC[0];
    //}
    if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        captureLow2 = TC4_REGS->COUNT16.TC_CC[0];
        captureHigh2 = TC4_REGS->COUNT16.TC_CC[1];
    }

    TC4_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}
