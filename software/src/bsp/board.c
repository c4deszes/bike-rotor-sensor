#include "bsp/board.h"
#include "bsp/osh_phy.h"

#include "hal/sysctrl.h"
#include "hal/gclk.h"

#include "atsamd21e18a.h"

eic_configuration bsp_eic_config;

void BSP_Initialize(void) {
    TCC0_REGS->TCC_INTENSET = TCC_INTENSET_OVF_Msk;
    TCC0_REGS->TCC_PER = TCC_PER_PER(1000);

    TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
}

void DFLL_Init(uint8_t generator);

static void DFLL_Initialize(void)
{
    /* Remove the OnDemand mode, Bug http://avr32.icgroup.norway.atmel.com/bugzilla/show_bug.cgi?id=9905 */
    SYSCTRL_REGS->SYSCTRL_DFLLCTRL = SYSCTRL_DFLLCTRL_ENABLE_Msk;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk);

    // TODO: (VARIANT_MCK + VARIANT_MAINOSC/2) / VARIANT_MAINOSC
    // 48000
    SYSCTRL_REGS->SYSCTRL_DFLLMUL = SYSCTRL_DFLLMUL_MUL(1464) | SYSCTRL_DFLLMUL_FSTEP(511) | SYSCTRL_DFLLMUL_CSTEP(31);

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk);

    /*Load Calibration Value*/
    uint8_t calibCoarse = (uint8_t)(((*(uint32_t*)0x806024) >> 26 ) & 0x3f);
    calibCoarse = (((calibCoarse) == 0x3F) ? 0x1F : (calibCoarse));
    uint16_t calibFine = (uint16_t)(((*(uint32_t*)0x806028)) & 0x3ff);

    SYSCTRL_REGS->SYSCTRL_DFLLVAL = SYSCTRL_DFLLVAL_COARSE(calibCoarse) | SYSCTRL_DFLLVAL_FINE(calibFine);
    SYSCTRL_REGS->SYSCTRL_DFLLMUL = SYSCTRL_DFLLMUL_CSTEP( 31 ) | // Coarse step is 31, half of the max value
                                    SYSCTRL_DFLLMUL_FSTEP( 511 ) |
                                    SYSCTRL_DFLLMUL_MUL( (1464) ) ;

    SYSCTRL_REGS->SYSCTRL_DFLLCTRL = 0;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk);

    SYSCTRL_REGS->SYSCTRL_DFLLCTRL = SYSCTRL_DFLLCTRL_CCDIS_Msk |
                                     SYSCTRL_DFLLCTRL_MODE_Msk |
                                     SYSCTRL_DFLLCTRL_BPLCKC_Msk;

    SYSCTRL_REGS->SYSCTRL_DFLLCTRL |= SYSCTRL_DFLLCTRL_ENABLE_Msk;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLLCKF_Msk) != SYSCTRL_PCLKSR_DFLLLCKF_Msk);
}

void BSP_ClockInitialize (void)
{
    NVMCTRL_REGS->NVMCTRL_CTRLB |= NVMCTRL_CTRLB_RWS_HALF_Val ;

    // TODO: ABPAMASK
    // PM_REGS->PM_APBAMASK = PM_APBAMASK_Msk;
    SYSCTRL_EnableInternalOSC32K();

    GCLK_Reset();

    GCLK_ConfigureGenerator(GCLK_GEN1, GCLK_GENCTRL_SRC_OSC32K_Val, 0u);    // GCLK1 -> DFLL source
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_DFLL48_Val, GCLK_GEN1);
    DFLL_Initialize();                                                      // DFLL -> ~48MHz
    GCLK_ConfigureGenerator(GCLK_GEN0, GCLK_GENCTRL_SRC_DFLL48M_Val, 0u);   // DFLL -> MCLK

    SYSCTRL_ConfigureOSC8M();

    GCLK_ConfigureGenerator(GCLK_GEN2, GCLK_GENCTRL_SRC_OSCULP32K_Val, 0u); // GCLK2 -> 32kHz (low power)
    GCLK_ConfigureGenerator(GCLK_GEN3, GCLK_GENCTRL_SRC_OSC8M_Val, 0u);     // GCLK3 -> 8MHz
    GCLK_ConfigureGenerator(GCLK_GEN4, GCLK_GENCTRL_SRC_OSC8M_Val, 8u);     // GCLK4 -> 1MHz

    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EIC_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_WDT_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_0_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_1_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_2_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_3_Val, GCLK_GEN3);

    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_TCC2_TC3_Val, GCLK_GEN4);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_TC4_TC5_Val, GCLK_GEN4);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_TCC0_TCC1_Val, GCLK_GEN4);     // Scheduler

    NVIC_SetPriority(TCC0_IRQn, 3);
    NVIC_EnableIRQ(TCC0_IRQn);

    //GCLK_SelectGenerator(GCLK_CLKCTRL_ID_SERCOM0_CORE_Val, GCLK_GEN3);

    PM_REGS->PM_CPUSEL = PM_CPUSEL_CPUDIV_DIV1;
    PM_REGS->PM_APBASEL = PM_APBASEL_APBADIV_DIV1;
    PM_REGS->PM_APBBSEL = PM_APBBSEL_APBBDIV_DIV1;
    PM_REGS->PM_APBCSEL = PM_APBCSEL_APBCDIV_DIV1;

    /* Configure the APBC Bridge Clocks */
    //PM_REGS->PM_APBCMASK = 0x11806 | PM_APBCMASK_TCC0_Msk;
    PM_REGS->PM_APBCMASK = PM_APBCMASK_EVSYS_Msk |
                           PM_APBCMASK_TC3_Msk |
                           PM_APBCMASK_TC4_Msk |
                           PM_APBCMASK_TC5_Msk |
                           PM_APBCMASK_TCC0_Msk;
}
