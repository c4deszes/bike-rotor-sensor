#include "app/sys_state.h"
#include "common/swtimer.h"
#include "app/sec.h"
#include "app/comm.h"
#include "app/feature.h"
#include "app/ridelogs.h"
#include "app/ride.h"
#include "app/diagnostics.h"

#include "bl/api.h"

#include "sam.h"

typedef enum {
    sys_state_normal,
    sys_state_goto_boot,
    sys_state_goto_reset,
    sys_state_goto_sleep
} sys_state_t;

static sys_state_t sys_state;
static swtimer_t* transition_timer;

void SYSSTATE_Initialize(void) {
    sys_state = sys_state_normal;
    transition_timer = SWTIMER_Create();
}

uint64_t boot_entry_key __attribute__((section(".bl_shared_ram")));
static void SYSSTATE_BootEntry(void) {
    boot_entry_key = BOOT_ENTRY_MAGIC;

    NVIC_SystemReset();
}

static void SYSSTATE_Reset(void) {
    NVIC_SystemReset();
}

void SYSSTATE_Update(void) {
    if (sys_state == sys_state_normal) {
        if(DIAG_BootRequest()) {
            sys_state = sys_state_goto_boot;
            SWTIMER_Setup(transition_timer, SYS_STATE_BOOT_ENTRY_DELAY);
        }
    }
    else if (sys_state == sys_state_goto_boot) {
        if (SWTIMER_Elapsed(transition_timer)) {
            SYSSTATE_BootEntry();
        }
    }
    else if (sys_state == sys_state_goto_reset) {
        if (SWTIMER_Elapsed(transition_timer)) {
            SYSSTATE_Reset();
        }
    }
    else if (sys_state == sys_state_goto_sleep) {
        // TODO: sleep support
        RIDE_Stop();
        RIDELOGS_SaveContainer();
    }
}
