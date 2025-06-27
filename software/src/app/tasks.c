#include "common/scheduler.h"
#include "hal/wdt.h"

#include <stdint.h>

#include "app/comm.h"
#include "app/sec.h"
#include "app/speed.h"
#include "app/imu.h"
#include "app/itpms.h"
#include "app/sys_state.h"
#include "common/swtimer.h"
#include "bsp/sensor.h"
#include "app/cadence.h"
#include "app/ride.h"
#include "app/ridelogs.h"

#include "bsp/usart.h"
#include "app/config.h"

#include "app/altitude.h"
#include "app/gear.h"
#include "app/distance.h"
#include "app/current.h"
#include "app/volt.h"
#include "app/diagnostics.h"

// void SCH_Task100us(void) {
//     //COMM_UpdatePhy();
// }

void SCH_Task1ms(void) {
    SWTIMER_Update1ms();

    SEC_Update();       // Could be moved to 10ms task

    if (SENSOR_HasData(SPM_FRONT_SENSOR_CHANNEL)) {
        osh_sensor_sample_t sample = SENSOR_GetSample(SPM_FRONT_SENSOR_CHANNEL);
        SPEED_OnTick(SPM_FRONT_SENSOR_CHANNEL, sample);
        DIST_OnTick(SPM_FRONT_SENSOR_CHANNEL, sample);
        // TODO: call ITPMS_OnTick
        GEAR_OnTick(SPM_FRONT_SENSOR_CHANNEL, sample);
    }

    if (SENSOR_HasData(SPM_REAR_SENSOR_CHANNEL)) {
        osh_sensor_sample_t sample = SENSOR_GetSample(SPM_REAR_SENSOR_CHANNEL);
        SPEED_OnTick(SPM_REAR_SENSOR_CHANNEL, sample);
        DIST_OnTick(SPM_REAR_SENSOR_CHANNEL, sample);
        // TODO: call ITPMS_OnTick
        GEAR_OnTick(SPM_REAR_SENSOR_CHANNEL, sample);
    }

    if (SENSOR_HasData(SPM_CRANK_SENSOR_CHANNEL)) {
        osh_sensor_sample_t sample = SENSOR_GetSample(SPM_CRANK_SENSOR_CHANNEL);
        CAD_OnTick(SPM_CRANK_SENSOR_CHANNEL, sample);
        GEAR_OnTick(SPM_CRANK_SENSOR_CHANNEL, sample);
    }

    COMM_UpdatePhy();
}

void SCH_Task10ms_A(void) {
    // TODO: enable watchdog
    //WDT_Acknowledge();

    SPEED_Update();

    IMU_Update();

    CAD_Update();

    ITPMS_Update();

    SYSSTATE_Update();

    COMM_UpdateFastSignals();

    COMM_UpdateDebugSignals();
}

void SCH_Task100ms(void) {
    ALT_Update();

    GEAR_Update();

    VOLT_Update();

    RIDELOGS_UpdateActiveRecord();

    COMM_UpdateSlowSignals();
}

void SCH_Task1000ms() {
    RIDE_Update();

    DIST_Update();      // Only distance status is updated here

    CURRENT_Update();

    DIAG_Update();
}