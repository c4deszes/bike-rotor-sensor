#include "bootloader/bootloader_internal.h"

int main(void) {
    /** Initialize configuration */
    boot_init();

    /** Enter bootloader loop */
    boot_enter();
}
