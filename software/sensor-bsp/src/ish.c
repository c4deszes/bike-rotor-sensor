#include "board/ish.h"
#include "board/ish_internal.h"

#include "hal/sys.h"
#include "hal/tca.h"
#include "hal/tcb.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFFER_SIZE 32

static uint8_t pos_buffer[BUFFER_SIZE];
static uint8_t neg_buffer[BUFFER_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

void ish_put(uint8_t pos, uint8_t neg) {
    pos_buffer[head] = pos;
    neg_buffer[head] = neg;
    head = (head + 1) % BUFFER_SIZE;
}

uint8_t ish_available() {
    return ((uint8_t)(BUFFER_SIZE + head - tail)) % BUFFER_SIZE;
}

ish_data ish_get() {
    if (head == tail) {
        ish_data data = {0xFF, 0xFF};
        return data;
    }
    ish_data data = {pos_buffer[tail], neg_buffer[tail]};
    tail = (tail + 1) % BUFFER_SIZE;
    return data;
}

void tca_overflow_handler() {
    //ish_put(0, 0xFF);
}

void tcb_capture_handler(const uint16_t capture, const uint16_t count) {
    uint8_t neg = ((uint32_t)capture) * 1000 / 250000;
    uint8_t pos = ((uint32_t)(count - capture)) * 1000 / 250000;
    ish_put(pos, neg);
}

static const tca_configuration tca_config = {
    .clksel = TCA_CLKSEL_CLKDIV1
};

void ish_tcb_init() {
    tca_init(&tca_config);
    tca_set_period(63750);  // 255ms
    tca_enable_overflow_interrupt();

    tcb_init((void*)0);
    tcb_enable_interrupt();
}