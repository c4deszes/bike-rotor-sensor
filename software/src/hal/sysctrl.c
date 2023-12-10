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

void SYSCTRL_InitializeDFLL(uint32_t freq_in, uint32_t freq_out) {
    /* Remove the OnDemand mode, Bug http://avr32.icgroup.norway.atmel.com/bugzilla/show_bug.cgi?id=9905 */
    SYSCTRL_REGS->SYSCTRL_DFLLCTRL = SYSCTRL_DFLLCTRL_ENABLE_Msk;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk);

    uint16_t dfll_mul = (uint16_t) ((freq_out + (freq_in / 2)) / freq_in);

    SYSCTRL_REGS->SYSCTRL_DFLLMUL = SYSCTRL_DFLLMUL_MUL(dfll_mul) | SYSCTRL_DFLLMUL_FSTEP(511) | SYSCTRL_DFLLMUL_CSTEP(31);

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk);

    /*Load Calibration Value*/
    uint8_t calibCoarse = (uint8_t)(((*(uint32_t*)0x806024) >> 26 ) & 0x3f);
    calibCoarse = (((calibCoarse) == 0x3F) ? 0x1F : (calibCoarse));
    uint16_t calibFine = (uint16_t)(((*(uint32_t*)0x806028)) & 0x3ff);

    SYSCTRL_REGS->SYSCTRL_DFLLVAL = SYSCTRL_DFLLVAL_COARSE(calibCoarse) | SYSCTRL_DFLLVAL_FINE(calibFine);
    SYSCTRL_REGS->SYSCTRL_DFLLMUL = SYSCTRL_DFLLMUL_CSTEP( 31 ) | // Coarse step is 31, half of the max value
                                    SYSCTRL_DFLLMUL_FSTEP( 511 ) |
                                    SYSCTRL_DFLLMUL_MUL( (dfll_mul) ) ;

    SYSCTRL_REGS->SYSCTRL_DFLLCTRL = 0;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk);

    SYSCTRL_REGS->SYSCTRL_DFLLCTRL = SYSCTRL_DFLLCTRL_CCDIS_Msk |
                                     SYSCTRL_DFLLCTRL_MODE_Msk |
                                     SYSCTRL_DFLLCTRL_BPLCKC_Msk;

    SYSCTRL_REGS->SYSCTRL_DFLLCTRL |= SYSCTRL_DFLLCTRL_ENABLE_Msk;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLLCKF_Msk) != SYSCTRL_PCLKSR_DFLLLCKF_Msk);
}
