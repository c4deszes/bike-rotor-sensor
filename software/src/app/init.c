#include "atsamd21e18a.h"

#include "bsp/board.h"
#include "hal/rtc.h"
#include "hal/wdt.h"
#include "hal/nvic.h"

#include "app/osh.h"
#include "bsp/osh_phy.h"

#include "app/sch.h"
#include <stddef.h>

//#include "hal/sercom0_usart.h"

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

wdt_normal_configuration wdt_config = {
    .period = WDT_TIMEOUT_CYC16384
};

void APP_Initialize() {
    BSP_ClockInitialize();

    //WDT_InitializeNormal(&wdt_config);

    BSP_Initialize();

    EIC_Initialize(NULL);

    //SERCOM0_USART_Initialize();

    OSH_Initialize();

    SCH_Init();

    OSH_TurnOn();
    OSH_Update();

    TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_SWRST_Msk;
    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_SWRST_Msk) != 0);

    TCC0_REGS->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1;
    TCC0_REGS->TCC_INTENSET = TCC_INTENSET_OVF_Msk;
    TCC0_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;
    TCC0_REGS->TCC_PER = 1000;

    NVIC_SetPriority(TCC0_IRQn, 3);
    NVIC_EnableIRQ(TCC0_IRQn);

    TCC0_REGS->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;

    while((TCC0_REGS->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) != 0);

    NVIC_Initialize();
}

void TCC0_Handler(void) {
    SCH_Trigger();

    TCC0_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;
}

// void EIC_Handler(void) {
//     EIC_REGS->EIC_INTFLAG = EIC_INTFLAG_Msk;
// }
