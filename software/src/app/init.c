#include "bsp/board.h"
#include "hal/rtc.h"
#include "hal/wdt.h"
#include "hal/nvic.h"
#include "hal/tcc.h"
#include "app/sec.h"
#include "app/speed.h"
#include "bsp/osh_phy.h"
#include "bsp/sensor.h"
#include "bsp/bmp581.h"
#include "app/sch.h"
#include "app/comm.h"
#include "app/imu.h"
#include "app/cadence.h"
#include "app/distance.h"
#include "app/gear.h"
#include "app/itpms.h"
#include "app/sys_state.h"

#include "app/altitude.h"

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

void APP_Initialize() {
    // Low level init
    BSP_ClockInitialize();
    //WDT_InitializeNormal(&wdt_config);
    //EIC_Initialize(NULL);
    OSH_PhyInit();
    SENSOR_Initialize();
    BMP581_Initialize();
    BMI088_Initialize();

    // Initializing application services
    SEC_Initialize();

    SPEED_Initialize();
    IMU_Initialize();

    ALT_Initialize();
    CAD_Initialize();
    DIST_Initialize();
    GEAR_Initialize();
    ITPMS_Initialize();

    SYSSTATE_Initialize();

    // Initializing communication
    COMM_Initialize();

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
