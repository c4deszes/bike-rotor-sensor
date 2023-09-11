#include "hal/sysctrl.h"

#include "atsamd21e18a.h"

void SYSCTRL_EnableInternalOSC32K(void) {
    // TODO: use fuse address constants
    uint32_t calibValue = (uint32_t)(((*(uint64_t*)0x806020) >> 38 ) & 0x7f);
    /* Configure 32K RC oscillator */
    SYSCTRL_REGS->SYSCTRL_OSC32K = SYSCTRL_OSC32K_CALIB(calibValue) |
                                   SYSCTRL_OSC32K_STARTUP(6u) |
                                   SYSCTRL_OSC32K_ENABLE_Msk |
                                   SYSCTRL_OSC32K_EN32K_Msk ;

    while(!((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_OSC32KRDY_Msk) == SYSCTRL_PCLKSR_OSC32KRDY_Msk));
}

void SYSCTRL_ConfigureOSC8M(void) {
    /* Configure 8MHz Oscillator */
    SYSCTRL_REGS->SYSCTRL_OSC8M = (SYSCTRL_REGS->SYSCTRL_OSC8M & (SYSCTRL_OSC8M_CALIB_Msk | SYSCTRL_OSC8M_FRANGE_Msk)) | SYSCTRL_OSC8M_ENABLE_Msk | SYSCTRL_OSC8M_PRESC(0x0) ;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_OSC8MRDY_Msk) != SYSCTRL_PCLKSR_OSC8MRDY_Msk);
}
