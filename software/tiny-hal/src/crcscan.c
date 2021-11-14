#include "hal/crcscan.h"

#include <avr/io.h>

void crcscan_enable(void) {
    /** Mode must be reset to 0 after normal code start */
    CRCSCAN.CTRLB &= ~(CRCSCAN_MODE_gm);
    CRCSCAN.CTRLA = CRCSCAN_ENABLE_bm;
}

void crcscan_reset(void) {
    CRCSCAN.CTRLA = CRCSCAN_RESET_bm;
}

void crcscan_enable_interrupt(void) {
    CRCSCAN.CTRLA |= CRCSCAN_NMIEN_bm;
}

void crcscan_set_source(const crcscan_source source) {
    CRCSCAN.CTRLB |= (source & CRCSCAN_SRC_gm);
}

bool crcscan_is_ok(void) {
    return (CRCSCAN.STATUS & CRCSCAN_OK_bm) == 0 ? false : true;
}

bool crcscan_is_busy(void) {
    return (CRCSCAN.STATUS & CRCSCAN_BUSY_bm) == 0 ? false : true;
}
