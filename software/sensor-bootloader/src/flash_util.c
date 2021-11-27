#include "bootloader/flash_util.h"
#include "bootloader/memory.h"

#include <stddef.h>
#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/pgmspace.h>

void boot_erase_section(uint8_t section) {
    /** Wait for previous command */
    while(NVMCTRL.STATUS & NVMCTRL_FBUSY_bm);

    /** Page erase command */
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_PAGEERASE_gc);

    /** Calculate the application page address and perform dummy write */
    volatile uint8_t* addr = (uint8_t*)(__application_start__ + section * 256);
    *addr = 0x00;

    /** Wait for erase operation to complete */
    while(NVMCTRL.STATUS & NVMCTRL_FBUSY_bm);
}

void boot_write_section() {
    /** Wait for previous command */
    while(NVMCTRL.STATUS & NVMCTRL_FBUSY_bm);

    /** Page write command */
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_NONE_gc);
    _PROTECTED_WRITE_SPM(NVMCTRL.CTRLA, NVMCTRL_CMD_PAGEWRITE_gc);
}
