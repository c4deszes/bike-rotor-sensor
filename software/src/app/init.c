#include "bsp/board.h"
#include "hal/rtc.h"
#include "hal/wdt.h"
#include "hal/nvic.h"
#include "hal/tcc.h"
#include "app/sec.h"
#include "app/spm.h"
#include "bsp/osh_phy.h"
#include "app/sch.h"

#include "hal/gpio.h"

#include <stddef.h>

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

const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = false
};

void APP_Initialize() {
    // Low level init
    BSP_ClockInitialize();
    //WDT_InitializeNormal(&wdt_config);
    EIC_Initialize(NULL);
    OSH_PhyInit();
    //SERCOM0_USART_Initialize();
    GPIO_SetupPinOutput(PORT_GROUP_A, 18, &output);

    // Initializing application services
    SEC_Initialize();
    //SPM_Initialize();
    //IET_Initialize();

    // Initializing communication
    //COMM_Initialize();

    // Setting up scheduler
    SCH_Init();
    TCC_Reset(TCC0);
    TCC_SetupTrigger(TCC0, 1000);   // 1000us period
    TCC_Enable(TCC0);

    NVIC_Initialize();
}

void TCC0_Interrupt(void) {
    SCH_Trigger();
}
