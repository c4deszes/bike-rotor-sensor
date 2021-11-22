#include "app/init.h"

#include "hal/sys.h"
#include "hal/sch.h"

int app_main(void) {
    /** Initialize configuration */
    app_init();

    sys_enable_interrupts();

    sch_init();

    /** Enter scheduler */
    sch_enter();

    return 0;
}

int main(void) __attribute__((weak, alias("app_main")));
