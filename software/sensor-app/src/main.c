#include "app/init.h"

#include "hal/sys.h"
#include "hal/evsys.h"
#include "hal/tcb.h"
#include "hal/sch.h"
#include "hal/usart.h"
#include "hal/portmux.h"

#include "board/ish.h"
#include "board/osh.h"

#include <avr/io.h>
#include <avr/interrupt.h>

ISR(TCB0_INT_vect) {
    //PORTB.OUTTGL = _BV(5);
    uint16_t posedge = TCB0.CNT;
    uint16_t negedge = TCB0.CCMP;
    TCB0.INTFLAGS = TCB_CAPT_bm;
}

int main(void) {
    // Enable user peripherals
    PORTB.OUTSET = _BV(5);              /** Turn off User LED */
    PORTB.DIRSET = _BV(5);              /** Initialize User LED */

    app_init();

    osh_output_on();

    ish_enable();

    evsys_user_async_select(0, 0x3);

    sys_enable_interrupts();

    /** Enter scheduler */
    sch_init();

    sch_enter();
}
