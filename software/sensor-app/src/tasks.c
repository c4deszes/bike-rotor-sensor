#include "hal/wdt.h"

#include <stdio.h>

static uint16_t cnt = 0;

void sch_task1ms(void) {

}

void sch_task10ms(void) {
    wdt_acknowledge();

    if (cnt == 100) {
        printf("123456789123456789\r\n");
        cnt = 0;
    }
    cnt++;
}
