#include "app/init.h"

#include "common/scheduler.h"

#include "hal/rtc.h"
#include "hal/wdt.h"
#include "hal/nvic.h"
#include "hal/tcc.h"

#include "bsp/board.h"
#include "bsp/osh_phy.h"
#include "bsp/sensor.h"
#include "bsp/bmp581.h"
#include "bsp/bmi088.h"

#include "app/config.h"
#include "app/comm.h"
#include "app/imu.h"
#include "app/cadence.h"
#include "app/diagnostics.h"
#include "app/distance.h"
#include "app/gear.h"
#include "app/itpms.h"
#include "app/sys_state.h"
#include "app/ride.h"
#include "app/ridelogs.h"
#include "app/volt.h"
#include "app/current.h"
#include "app/sec.h"
#include "app/speed.h"
#include "app/altitude.h"

#include <stddef.h>

wdt_normal_configuration wdt_config = {
    .period = WDT_TIMEOUT_CYC16384
};

void APP_Initialize() {
    // Low level init
    BSP_ClockInitialize();
    // TODO: enable watchdog
    //WDT_InitializeNormal(&wdt_config);
    //EIC_Initialize(NULL);
    OSH_PhyInit();
    SENSOR_Initialize();
    BMP581_Initialize();
    BMI088_Initialize();

    // Sensor stack initialization
    SEC_Initialize();
    SPEED_Initialize();
    IMU_Initialize();
    ALT_Initialize();
    VOLT_Initialize();
    CURRENT_Initialize();

    // Software functions
    DIST_Initialize();
    CAD_Initialize();
    GEAR_Initialize();
    ITPMS_Initialize();
    RIDE_Initialize();
    RIDELOGS_Initialize();

    SYSSTATE_Initialize();

    // Initializing communication
    COMM_Initialize();
    DIAG_Initialize();

    // Load configuration
    // TODO: is this the right sequence?
    //CONFIG_LoadFlashProperties();
    CONFIG_ReloadUdsProperties();

    // Setting up scheduler
    // TODO: replace with SysTick
    SCH_Init();
    TCC_Reset(TCC0);
    TCC_SetupTrigger(TCC0, 1000);   // 1000us period
    TCC_Enable(TCC0);

    NVIC_Initialize();
}

void TCC0_Interrupt(void) {
    SCH_Trigger();
}
