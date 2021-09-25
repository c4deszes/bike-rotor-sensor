#include "hal/ac.h"

#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

ac_compare_handler local_handler;

void ac_init(const ac_configuration* configuration) {
    uint8_t ctrla = 0;

    if (configuration->runstandy) {
        ctrla |= AC_RUNSTDBY_bm;
    }
    if (configuration->output_enable) {
        ctrla |= AC_OUTEN_bm;
    }
    ctrla |= (configuration->intmode << AC_INTMODE_gp) & AC_INTMODE_gm;
    ctrla |= (configuration->hysmode << AC_HYSMODE_gp) & AC_HYSMODE_gm;
    AC0.CTRLA = ctrla;
}

void ac_enable(void) {
    AC0.CTRLA |= AC_ENABLE_bm;
}

void ac_disable(void) {
    AC0.CTRLA &= ~(AC_ENABLE_bm);
}

void ac_setmux(const bool invert, const ac_mux_positive positive, const ac_mux_negative negative) {
    uint8_t muxctrla = 0;

    muxctrla |= invert ? AC_INVERT_bm : 0;
    muxctrla |= (positive << AC_MUXPOS0_bp);
    muxctrla |= (negative << AC_MUXNEG0_bp);

    AC0.MUXCTRLA = muxctrla;
}

void ac_interrupt_enable(ac_compare_handler handler) {
    AC0.INTCTRL &= ~AC_CMP_bm;

    local_handler = handler;

    AC0.INTCTRL |= AC_CMP_bm;
}

void ac_interrupt_disable() {
    AC0.INTCTRL &= ~(AC_CMP_bm);
}

bool ac_get_state() {
    return (AC0.STATUS & AC_STATE_bm) ? true: false;
}

ISR(AC0_AC_vect) {
    local_handler(ac_get_state());
    AC0.STATUS &= ~(AC_CMP_bm);
}
