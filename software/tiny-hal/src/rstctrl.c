#include "hal/rstctrl.h"

#include <avr/io.h>

rstctrl_cause rstctrl_get_cause() {
    if ((RSTCTRL.RSTFR & RSTCTRL_UPDIRF_bm) != 0) {
        return RSTCTRCL_CAUSE_UPDI;
    }
    else if ((RSTCTRL.RSTFR & RSTCTRL_SWRF_bm) != 0) {
        return RSTCTRCL_CAUSE_SOFTWARE;
    }
    else if ((RSTCTRL.RSTFR & RSTCTRL_WDRF_bm) != 0) {
        return RSTCTRCL_CAUSE_WATCHDOG;
    }
    else if ((RSTCTRL.RSTFR & RSTCTRL_EXTRF_bm) != 0) {
        return RSTCTRCL_CAUSE_EXTERNAL;
    }
    else if ((RSTCTRL.RSTFR & RSTCTRL_BORF_bm) != 0) {
        return RSTCTRCL_CAUSE_BROWN_OUT;
    }
    else if ((RSTCTRL.RSTFR & RSTCTRL_PORF_bm) != 0) {
        return RSTCTRCL_CAUSE_POWER_ON;
    }
    return RSTCTRCL_CAUSE_POWER_ON;
}

void rstctrl_software_reset(void) {
    RSTCTRL.SWRR = RSTCTRL_SWRE_bm;
}
