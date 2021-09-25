#include "hal/wdt.h"

#include <avr/io.h>

static uint16_t cnt = 0;

void sch_task100us(void) {
    //PORTB.OUTTGL = _BV(5);
}

void sch_task1ms(void) {
    // if (cnt > 100) {
    //     PORTB.OUTTGL = _BV(5);
    //     cnt = 0;
    // }
    // cnt ++;
}

void sch_task10ms(void) {
    // wdt_acknowledge();
    if (cnt > 100) {
        PORTB.OUTTGL = _BV(5);
        cnt = 0;
    }
    cnt ++;
}
