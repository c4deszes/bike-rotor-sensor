#include "app/init.h"

#include "hal/sys.h"
#include "hal/sch.h"

#include <avr/io.h>

int main(void) {
    PORTB.DIRSET = _BV(5);
    PORTB.OUTCLR = _BV(5);

    /** Initialize configuration */
    app_init();

    sys_enable_interrupts();

    sch_init();

    /** Enter scheduler */
    sch_enter();
}
