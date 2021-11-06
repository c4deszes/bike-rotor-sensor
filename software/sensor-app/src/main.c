#include "app/init.h"

#include "hal/sys.h"
#include "hal/sch.h"

#include <avr/io.h>

int main(void) {
    /** Initialize configuration */
    app_init();

    sys_enable_interrupts();

    sch_init();

    /** Enter scheduler */
    sch_enter();
}
