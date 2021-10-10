#include "hal/tcb.h"

#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

tcb_capture_callback local_callback = NULL;

void tcb_init(const tcb_configuration* configuration) {
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc << TCB_CLKSEL_gp;
    TCB0.CTRLB = TCB_CNTMODE_FRQPW_gc << TCB_CNTMODE_gp;
    TCB0.EVCTRL = TCB_CAPTEI_bm;
}

void tcb_enable(void) {
    TCB0.CTRLA |= TCB_ENABLE_bm;
}

void tcb_disable(void) {
    TCB0.CTRLA &= ~(TCB_ENABLE_bm);
}

void tcb_enable_interrupt(tcb_capture_callback callback) {
    local_callback = callback;
    TCB0.INTCTRL |= TCB_CAPT_bm;
}

void tcb_disable_interrupt(void) {
    TCB0.INTCTRL &= ~(TCB_CAPT_bm);
}

bool tcb_is_running(void) {
    return TCB0.STATUS & TCB_RUN_bm;
}
