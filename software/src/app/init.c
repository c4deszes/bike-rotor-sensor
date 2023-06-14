#include "atsamd21e18a.h"

#include "bsp/board.h"
#include "hal/rtc.h"

#include "bsp/osh_phy.h"

// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
// #pragma config NVMCTRL_BOOTPROT = SIZE_0BYTES
// #pragma config NVMCTRL_EEPROM_SIZE = SIZE_0BYTES
// #pragma config BOD33USERLEVEL = 0x7 // Enter Hexadecimal value
// #pragma config BOD33_EN = ENABLED
// #pragma config BOD33_ACTION = RESET

// #pragma config BOD33_HYST = DISABLED
// #pragma config NVMCTRL_REGION_LOCKS = 0xffff // Enter Hexadecimal value

// #pragma config WDT_ENABLE = DISABLED
// #pragma config WDT_ALWAYSON = DISABLED
// #pragma config WDT_PER = CYC16384

// #pragma config WDT_WINDOW_0 = SET
// #pragma config WDT_WINDOW_1 = 0x4 // Enter Hexadecimal value
// #pragma config WDT_EWOFFSET = CYC16384
// #pragma config WDT_WEN = DISABLED

void APP_Initialize() {
    BSP_ClockInitialize();

    BSP_Initialize();

    EIC_Initialize(&bsp_eic_config);

    //EIC_EnableInterrupt(4);
    //EIC_EnableInterrupt(5);

    //RTC_Initialize();

    __DMB();
    __enable_irq();

    TC4_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;

    osh_phy_turn_on();
}

void EIC_Handler(void) {
    EIC_REGS->EIC_INTFLAG = EIC_INTFLAG_Msk;
}
