#include "hal/tca.h"

#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

void tca_init(const tca_configuration* configuration) {
    TCA0.SINGLE.CTRLA = configuration->clksel << TCA_SINGLE_CLKSEL_gp;
}

void tca_enable(void) {
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void tca_disable(void) {
    TCA0.SINGLE.CTRLA &= ~(TCA_SINGLE_ENABLE_bm);
}

void tca_set_period(uint16_t period) {
    TCA0.SINGLE.PER = period;
}

void tca_set_count(uint16_t count) {
    TCA0.SINGLE.CNT = count;
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}

void tca_enable_overflow_interrupt() {
    TCA0.SINGLE.INTCTRL |= TCA_SINGLE_OVF_bm;
}

void tca_disable_overflow_interrupt(void) {
    TCA0.SINGLE.INTCTRL &= ~(TCA_SINGLE_OVF_bm);
}

ISR(TCA0_OVF_vect) {
    tca_overflow_handler();
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
