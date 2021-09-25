#include "hal/sys.h"

#include <avr/io.h>
#include <avr/interrupt.h>

extern void _fatal(void);

uint32_t sys_cpu_frequency = 0;

void sys_init(uint32_t cpu_frequency) {
    sys_cpu_frequency = cpu_frequency;
}

uint32_t sys_get_cpu_frequency(void) {
    return sys_cpu_frequency;
}

uint8_t sys_get_cpu_rev(void) {
    return SYSCFG.REVID;
}

void sys_enable_interrupts(void) {
    sei();
}

void sys_disable_interrupts(void) {
    cli();
}

/**
 * @brief Dummy handler
 */
ISR(BADISR_vect) {
    _fatal();
}
