#include "hal/nvmctrl.h"

#include <avr/io.h>
#include <avr/eeprom.h>

void nvmctrl_bootlock(void) {
    NVMCTRL.CTRLB |= NVMCTRL_BOOTLOCK_bm;
}
