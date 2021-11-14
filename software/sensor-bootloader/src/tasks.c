#include "hal/wdt.h"
#include "bootloader/bootloader_internal.h"

#include <avr/io.h>

static uint8_t cnt = 0;
static uint16_t boot_timer = 500;

void sch_task10ms(void) {
    wdt_acknowledge();

    if (cnt > 10) {
        PORTB.OUTTGL = _BV(5);
        cnt = 0;
    }

    boot_timer--;
    cnt ++;

    if (boot_timer <= 0) {
        boot_startapp();
    }
}
