#include "app/init.h"
#include "board/osh.h"

#include "hal/sys.h"
#include "hal/evsys.h"
#include "hal/tcb.h"
#include "hal/sch.h"
#include "board/ish.h"
#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TCB0_INT_vect) {
    // PORTB.OUTTGL = _BV(5);
    (uint16_t)TCB0.CCMP;
    TCB0.INTFLAGS = TCB_CAPT_bm;
}

ISR(TCA0_OVF_vect) {
    sch_trigger();
    // clear flag
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

int main(void) {
    // Enable user peripherals
    PORTB.OUTSET = _BV(5);              /** Turn off User LED */
    PORTB.DIRSET = _BV(5);              /** Initialize User LED */

    app_init();

    osh_output_on();

    ish_enable();

    // TCB0 user -> Async Generator 0
    evsys_user_async_select(0, 0x3);

    /**
     * @brief 
     * TCA calculations
     * 
     * 1 000 000 Hz -> 0.000001 / tick
     * div 4 -> 0.0000002 / tick
     * 100us = 0.000004s * 25
     * 100us period for scheduler
     * Compare
     */
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;
    TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
    TCA0.SINGLE.PER = 100;
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;

    tcb_init((void*)0);
    tcb_enable_interrupt((void*)0);
    tcb_enable();

    sys_enable_interrupts();

    PORTB.OUTCLR = _BV(5);

    sch_init();

    sch_enter();
}
