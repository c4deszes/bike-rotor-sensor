#include "app/sch.h"
#include "hal/wdt.h"
#include "app/osh.h"

#include <stdint.h>

#include "bsp/sensor.h"

void SCH_Task1ms(void) {
    if (SENSOR_HasData(0)) {
        osh_sensor_sample sample = SENSOR_GetData(0);
    }

    OSH_Update();
}

void SCH_Task10ms_A(void) {
    //WDT_Acknowledge();
}
