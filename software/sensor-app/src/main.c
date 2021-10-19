#include "app/init.h"

#include "hal/sys.h"
#include "hal/sch.h"

#include <avr/io.h>

int main(void) {
    // Enable user peripherals
    PORTB.OUTSET = _BV(5);              /** Turn off User LED */
    PORTB.DIRSET = _BV(5);              /** Initialize User LED */

    app_init();

    sys_enable_interrupts();

    /** Enter scheduler */
    sch_init();

    sch_enter();
}
