#include "bootloader/bootloader_internal.h"

#include "hal/sys.h"
#include "hal/sch.h"

int main(void) {
    /** Initialize configuration */
    boot_init();

    sys_enable_interrupts();

    sch_init();

    /** Enter scheduler */
    sch_enter();
}
