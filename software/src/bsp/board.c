#include "bsp/board.h"
#include "bsp/osh_phy.h"

#include "hal/sysctrl.h"
#include "hal/gclk.h"
#include "hal/pm.h"

#include "sam.h"

eic_configuration bsp_eic_config;

void BSP_Initialize(void) {
    // TCC0_REGS->TCC_INTENSET = TCC_INTENSET_OVF_Msk;
    // TCC0_REGS->TCC_PER = TCC_PER_PER(1000);

    // TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;

    // NVIC_SetPriority(TCC0_IRQn, 3);
    // NVIC_EnableIRQ(TCC0_IRQn);
}

void BSP_ClockInitialize (void) {
    NVMCTRL_REGS->NVMCTRL_CTRLB |= NVMCTRL_CTRLB_RWS_HALF_Val ;

    SYSCTRL_EnableInternalOSC32K();

    GCLK_Reset();

    GCLK_ConfigureGenerator(GCLK_GEN1, GCLK_GENCTRL_SRC_OSC32K_Val, 0u);    // GCLK1 -> DFLL source
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_DFLL48_Val, GCLK_GEN1);
    SYSCTRL_InitializeDFLL(32768u, 48000000u);                              // DFLL -> ~48MHz
    GCLK_ConfigureGenerator(GCLK_GEN0, GCLK_GENCTRL_SRC_DFLL48M_Val, 0u);   // DFLL -> MCLK

    SYSCTRL_ConfigureOSC8M();

    GCLK_ConfigureGenerator(GCLK_GEN2, GCLK_GENCTRL_SRC_OSCULP32K_Val, 0u); // GCLK2 -> 32kHz (low power)
    GCLK_ConfigureGenerator(GCLK_GEN3, GCLK_GENCTRL_SRC_OSC8M_Val, 0u);     // GCLK3 -> 8MHz
    GCLK_ConfigureGenerator(GCLK_GEN4, GCLK_GENCTRL_SRC_OSC8M_Val, 8u);     // GCLK4 -> 1MHz
    GCLK_ConfigureGenerator(GCLK_GEN5, GCLK_GENCTRL_SRC_OSC8M_Val, 400u >> 1);    // GCLK5 -> 20kHz

    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EIC_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_WDT_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_0_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_1_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_2_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_3_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_4_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_5_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_6_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_7_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_8_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_9_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_10_Val, GCLK_GEN3);
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_EVSYS_11_Val, GCLK_GEN3);

    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_ADC_Val, GCLK_GEN3);              // RTC

    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_TCC2_TC3_Val, GCLK_GEN5);      // Speed sensor
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_TC4_TC5_Val, GCLK_GEN5);       // Speed sensor
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_TCC0_TCC1_Val, GCLK_GEN4);     // Scheduler

    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_SERCOM0_CORE_Val, GCLK_GEN3);  // LINE
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_SERCOM1_CORE_Val, GCLK_GEN3);  // Pressure sensor
    GCLK_SelectGenerator(GCLK_CLKCTRL_ID_SERCOM3_CORE_Val, GCLK_GEN3);  // IMU

    //GCLK_SelectGenerator(GCLK_CLKCTRL_ID_ADC_Val, GCLK_GEN4);

    //GCLK_SelectGenerator(GCLK_CLKCTRL_ID_SERCOM0_CORE_Val, GCLK_GEN3);

    PM_SelectCpuDiv(PM_CPUSEL_CPUDIV_DIV1);
    PM_SelectBusDiv(PM_APBASEL_APBADIV_DIV1,
                    PM_APBBSEL_APBBDIV_DIV1,
                    PM_APBCSEL_APBCDIV_DIV1);

    /* Configure the APBC Bridge Clocks */
        
    // APBA peripherals (eic, rtc, wdt, gclk, sysctrl, pm, pac0) are enabled on reset
    // PM_REGS->PM_APBAMASK = PM_APBAMASK_Msk;
    
    // APBB peripherals (usb, dmac, port, nvmctrl, dsu, pac1) are enabled on reset
    // TODO: maybe disable usb?
    // PM_REGS->PM_APBBMASK = PM_APBBMASK_Msk;

    PM_REGS->PM_APBCMASK = PM_APBCMASK_EVSYS_Msk |
                           PM_APBCMASK_ADC_Msk |
                           PM_APBCMASK_TC3_Msk |
                           PM_APBCMASK_TC4_Msk |
                           PM_APBCMASK_TC5_Msk |
                           PM_APBCMASK_TCC0_Msk |
                           PM_APBCMASK_SERCOM0_Msk |
                           PM_APBCMASK_SERCOM1_Msk |
                           PM_APBCMASK_SERCOM3_Msk;
    // TODO: enable SERCOM in APBC
}
