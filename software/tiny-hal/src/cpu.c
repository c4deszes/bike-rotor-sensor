#include "hal/cpu.h"

#include <avr/io.h>

void ccp_write_io(volatile uint8_t* address, uint8_t data) {
    CCP = CCP_IOREG_gc;
    *address = data;
}
