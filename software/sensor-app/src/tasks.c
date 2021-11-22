#include "hal/wdt.h"
#include "hal/usart.h"

#include <stdio.h>

#include "board/board.h"
#include "board/ish.h"
#include "sensor/pse.h"
#include "sensor/spe.h"
#include "sensor/dsa.h"

void sch_task1ms(void) {
    // if (ish_available() > 0) {
    //     ish_data pulse = ish_get();
    //     dsa_update(pulse.positive, pulse.negative);
    //     pse_update(pulse.positive, pulse.negative);
    //     spe_update(pulse.positive, pulse.negative);
    //     usart_sync_write((uint8_t)spe_get_speed());
    // }
}

static uint8_t cnt = 0;

void sch_task10ms(void) {
    wdt_acknowledge();

    if (cnt > 200) {
        board_user_led_toggle();
        cnt = 0;
    }
    cnt ++;
}
