#include "hal/wdt.h"
#include "hal/usart.h"

#include <stdio.h>

#include "board/board.h"
#include "board/ish.h"
#include "sensor/pse.h"
#include "sensor/spe.h"
#include "sensor/dsa.h"

#include "board/pins.h"
#include "hal/port.h"
#include "hal/adc.h"

void sch_task1ms(void) {
    if (ish_available() > 0) {
        ish_data pulse = ish_get();
        dsa_update(pulse.positive, pulse.negative);
        pse_update(pulse.positive, pulse.negative);
        spe_update(pulse.positive, pulse.negative);
        //usart_sync_write((uint8_t)pulse.positive);
    }
}

static uint8_t cnt = 0;

void sch_task10ms(void) {
    wdt_acknowledge();

    if (cnt > 100) {
        board_user_led_toggle();

        // Read temperature
        adc_setmux(ADC_MUXPOS_TEMPSENSE);
        adc_start();
        uint8_t temp = adc_sync_read();
        usart_sync_write(temp);

        uint16_t speed = spe_get_speed();
        uint16_t pos = pse_get_position();
        uint32_t distance = dsa_get_rotations();

        usart_sync_write((uint8_t)(speed >> 8));
        usart_sync_write((uint8_t)(speed & 0xFF));

        usart_sync_write((uint8_t)(pos >> 8));
        usart_sync_write((uint8_t)(pos & 0xFF));

        usart_sync_write((uint8_t)((distance >> 24) & 0xFF));
        usart_sync_write((uint8_t)((distance >> 16) & 0xFF));
        usart_sync_write((uint8_t)((distance >> 8) & 0xFF));
        usart_sync_write((uint8_t)((distance & 0xFF)));

        usart_sync_write((uint8_t)(0x0A));

        cnt = 0;
    }

    cnt ++;
}
