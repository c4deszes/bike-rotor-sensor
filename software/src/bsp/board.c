#include "bsp/board.h"
#include "bsp/osh_phy.h"

#include "atsamd21e18a.h"

eic_configuration bsp_eic_config;

void BSP_Initialize(void) {
    osh_phy_init();

    //EIC_Initialize(&bsp_eic_config);
}
static void SYSCTRL_Initialize(void)
{

	/* Configure 8MHz Oscillator */
    SYSCTRL_REGS->SYSCTRL_OSC8M = (SYSCTRL_REGS->SYSCTRL_OSC8M & (SYSCTRL_OSC8M_CALIB_Msk | SYSCTRL_OSC8M_FRANGE_Msk)) | SYSCTRL_OSC8M_ENABLE_Msk | SYSCTRL_OSC8M_PRESC(0x0) ;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_OSC8MRDY_Msk) != SYSCTRL_PCLKSR_OSC8MRDY_Msk)
    {
        /* Waiting for the OSC8M Ready state */
    }
    /****************** OSC32K Initialization  ******************************/
    uint32_t calibValue = (uint32_t)(((*(uint64_t*)0x806020) >> 38 ) & 0x7f);
    /* Configure 32K RC oscillator */
    SYSCTRL_REGS->SYSCTRL_OSC32K = SYSCTRL_OSC32K_CALIB(calibValue) | SYSCTRL_OSC32K_STARTUP(5) | SYSCTRL_OSC32K_ENABLE_Msk | SYSCTRL_OSC32K_EN32K_Msk ;
    while(!((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_OSC32KRDY_Msk) == SYSCTRL_PCLKSR_OSC32KRDY_Msk))
    {
        /* Waiting for the OSC32K Ready state */
    }
}


static void DFLL_Initialize(void)
{
    /****************** DFLL Initialization  *********************************/
    SYSCTRL_REGS->SYSCTRL_DFLLCTRL &= ~SYSCTRL_DFLLCTRL_ONDEMAND_Msk;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLRDY_Msk) != SYSCTRL_PCLKSR_DFLLRDY_Msk)
    {
        /* Waiting for the Ready state */
    }

    /*Load Calibration Value*/
    uint8_t calibCoarse = (uint8_t)(((*(uint32_t*)0x806024) >> 26 ) & 0x3f);
    calibCoarse = (((calibCoarse) == 0x3F) ? 0x1F : (calibCoarse));
    uint16_t calibFine = (uint16_t)(((*(uint32_t*)0x806028)) & 0x3ff);

    SYSCTRL_REGS->SYSCTRL_DFLLVAL = SYSCTRL_DFLLVAL_COARSE(calibCoarse) | SYSCTRL_DFLLVAL_FINE(calibFine);

    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_GEN(0x1)  | GCLK_CLKCTRL_CLKEN_Msk | GCLK_CLKCTRL_ID(0);
    SYSCTRL_REGS->SYSCTRL_DFLLMUL = SYSCTRL_DFLLMUL_MUL(1464) | SYSCTRL_DFLLMUL_FSTEP(8) | SYSCTRL_DFLLMUL_CSTEP(8);


    /* Configure DFLL    */
    SYSCTRL_REGS->SYSCTRL_DFLLCTRL = SYSCTRL_DFLLCTRL_ENABLE_Msk | SYSCTRL_DFLLCTRL_MODE_Msk ;

    while((SYSCTRL_REGS->SYSCTRL_PCLKSR & SYSCTRL_PCLKSR_DFLLLCKF_Msk) != SYSCTRL_PCLKSR_DFLLLCKF_Msk)
    {
        /* Waiting for DFLL to fully lock to meet clock accuracy */
    }
}


static void GCLK0_Initialize(void)
{

    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(6) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(0);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 0 synchronization */
    }
}


static void GCLK1_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(4) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(1);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 1 synchronization */
    }
}


static void GCLK2_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(3) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(2);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 2 synchronization */
    }
}


static void GCLK3_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(6) | GCLK_GENCTRL_OE_Msk | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(3);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 3 synchronization */
    }
}


static void GCLK4_Initialize(void)
{
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(6) | GCLK_GENCTRL_RUNSTDBY_Msk | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(4);

    GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_DIV(8) | GCLK_GENDIV_ID(4);
    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk)
    {
        /* wait for the Generator 4 synchronization */
    }
}

void BSP_ClockInitialize (void)
{
    /* Function to Initialize the Oscillators */
    SYSCTRL_Initialize();

    GCLK3_Initialize();
    GCLK4_Initialize();
    GCLK1_Initialize();
    GCLK2_Initialize();
    DFLL_Initialize();
    GCLK0_Initialize();


    /* Selection of the Generator and write Lock for RTC */
    //GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(4) | GCLK_CLKCTRL_GEN(0x2)  | GCLK_CLKCTRL_CLKEN_Msk;
    
    /* Selection of the Generator and write Lock for EIC */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(5) | GCLK_CLKCTRL_GEN(0x3)  | GCLK_CLKCTRL_CLKEN_Msk;

    /* Selection of the Generator and write Lock for EVSYS_0 */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(7) | GCLK_CLKCTRL_GEN(0x3)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for EVSYS_1 */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(8) | GCLK_CLKCTRL_GEN(0x3)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for EVSYS_1 */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(9) | GCLK_CLKCTRL_GEN(0x3)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for EVSYS_1 */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(10) | GCLK_CLKCTRL_GEN(0x3)  | GCLK_CLKCTRL_CLKEN_Msk;

    /* Selection of the Generator and write Lock for TC3 TCC2 */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(27) | GCLK_CLKCTRL_GEN(0x4)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for TC4 TC5 */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(28) | GCLK_CLKCTRL_GEN(0x4)  | GCLK_CLKCTRL_CLKEN_Msk;
    /* Selection of the Generator and write Lock for ADC */
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(30) | GCLK_CLKCTRL_GEN(0x3)  | GCLK_CLKCTRL_CLKEN_Msk;

    /* Configure the APBC Bridge Clocks */
    PM_REGS->PM_APBCMASK = 0x11806;


}
