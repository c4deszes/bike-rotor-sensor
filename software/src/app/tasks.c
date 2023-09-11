#include "app/sch.h"
#include "hal/wdt.h"
#include "app/osh.h"

#include <stdint.h>

void SCH_Task1ms(void) {
    
}

void TCC0_Handler(void) {
    SCH_Trigger();
}

// void SCH_Task10ms_A(void) {
//     WDT_Acknowledge();
// }
