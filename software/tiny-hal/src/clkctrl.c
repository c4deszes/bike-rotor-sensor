#include "hal/clkctrl.h"

#include "hal/cpu.h"
#include <avr/io.h>

inline uint8_t clkctrl_get_status_mask(const clkctrl_clock_select clock) {
    switch (clock) {
        case CLKCTRL_CLOCK_SELECT_EXTCLK: return CLKCTRL_EXTS_bm;
        case CLKCTRL_CLOCK_SELECT_OSCULP32K: return CLKCTRL_OSC32KS_bm;
        case CLKCTRL_CLOCK_SELECT_OSC20M: return CLKCTRL_OSC20MS_bm;
        default: return 0xFF;
    }
}

void clkctrl_set_clock(const clkctrl_clock_select clock) {
    ccp_write_io(&(CLKCTRL.MCLKCTRLA), (clock & CLKCTRL_CLKSEL_gm));

    uint8_t status_mask = clkctrl_get_status_mask(clock);

    /* Wait for the clock to stabilize */
    while ((CLKCTRL.MCLKSTATUS & status_mask) == 0) {}

    /* Wait for the main clock change to finalize */
    while ((CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm) != 0) {}
}

void clkctrl_set_prescaler(const clkctrl_prescaler prescaler) {
    uint8_t ctrlb = 0;
    if (prescaler == CLKCTRL_PRESCALER_NODIV) {
        ctrlb = ((prescaler << CLKCTRL_PDIV_gp) & CLKCTRL_PDIV_gm);
    }
    else {
        ctrlb = ((prescaler << CLKCTRL_PDIV_gp) & CLKCTRL_PDIV_gm) | CLKCTRL_PEN_bm;
    }
    ccp_write_io(&(CLKCTRL.MCLKCTRLB), ctrlb);
}

void clkctrl_lock() {
    CLKCTRL.MCLKLOCK |= CLKCTRL_LOCKEN_bm;
}
