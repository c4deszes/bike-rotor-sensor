#include "app/sch.h"
#include "hal/wdt.h"

#include <stdint.h>

#include "app/comm.h"
#include "app/sec.h"
#include "app/spm.h"
#include "app/iet.h"
#include "app/itpms.h"
#include "app/sys_state.h"
#include "common/swtimer.h"

#include "bsp/usart.h"

#include "app/alt.h"

// void SCH_Task100us(void) {
//     //COMM_UpdatePhy();
// }

void SCH_Task1ms(void) {
    SWTIMER_Update1ms();

    //SEC_Update();

    COMM_UpdatePhy();

    //SPM_Update();

    SYSSTATE_Update();
}

void SCH_Task10ms_A(void) {
    //WDT_Acknowledge();

    IET_Update();

    ITPMS_Update();

    ALT_Update();

    COMM_UpdateSignals();

    COMM_UpdateDebugSignals();
}
