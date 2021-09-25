#include "hal/wdt.h"

#include <avr/io.h>

void wdt_init_normal(const wdt_period period) {
    WDT.CTRLA = period & WDT_PERIOD_gm;
    WDT.STATUS = WDT_LOCK_bm;

    /* Wait for synchronization */
    while ((WDT.STATUS & WDT_SYNCBUSY_bm) != 0);
}

void wdt_init_window(const wdt_period open, const wdt_period closed) {
    WDT.CTRLA = ((closed & WDT_WINDOW_gm) << WDT_WINDOW_gp) | (open & WDT_PERIOD_gm);
    WDT.STATUS = WDT_LOCK_bm;

    /* Wait for synchronization */
    while ((WDT.STATUS & WDT_SYNCBUSY_bm) != 0);
}

void wdt_acknowledge(void) {
    __asm__("wdr;");
}
