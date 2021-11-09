#include "hal/cpuint.h"
#include "hal/cpu.h"

#include <avr/io.h>

void cpuint_ivsel(cpuint_ivsel_location loc) {
    uint8_t tmp = CPUINT.CTRLA;

    if (loc == CPUINT_IVSEL_BOOT) {
        tmp |= CPUINT_IVSEL_bm;
    }
    else {
        tmp &= ~(CPUINT_IVSEL_bm);
    }
    ccp_write_io(&(CPUINT.CTRLA), tmp);
}
