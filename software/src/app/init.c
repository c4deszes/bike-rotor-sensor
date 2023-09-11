#include "atsamd21e18a.h"

#include "bsp/board.h"
#include "hal/rtc.h"
#include "hal/wdt.h"
#include "hal/nvic.h"

#include "app/osh.h"
#include "bsp/osh_phy.h"

#include "app/sch.h"

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

    EIC_Initialize(&bsp_eic_config);

    //SERCOM0_USART_Initialize();

    osh_init();

    SCH_Init();

    NVIC_Initialize();
}

// void EIC_Handler(void) {
//     EIC_REGS->EIC_INTFLAG = EIC_INTFLAG_Msk;
// }
