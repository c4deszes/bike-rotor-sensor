#include "hal/ac.h"

#include "atsamd21e18a.h"

void AC_Enable(void) {
    AC_REGS->AC_CTRLA = AC_CTRLA_ENABLE_Msk;
    // TODO: sync
}

void AC_Reset(void) {
    AC_REGS->AC_CTRLA = AC_CTRLA_SWRST_Msk;
    // TODO: sync
}
