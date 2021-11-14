#include "board/ish.h"
#include "board/ish_internal.h"

#include "hal/sys.h"
#include "hal/evsys.h"
#include "hal/tca.h"
#include "hal/tcb.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define BUFFER_SIZE 32

static bool overflow = false;
static uint8_t pos_buffer[BUFFER_SIZE];
static uint8_t neg_buffer[BUFFER_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

void ish_put(uint8_t pos, uint8_t neg) {
    uint8_t next = (head + 1) % BUFFER_SIZE;
    if (next == tail) {
        overflow = true;
        return;
    }
    pos_buffer[head] = pos;
    neg_buffer[head] = neg;
    head = next;
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

// void tcb_capture_handler(const uint16_t capture, const uint16_t count) {
//     uint8_t neg = ((uint32_t)capture) * 1000 / 250000;
//     uint8_t pos = ((uint32_t)(count - capture)) * 1000 / 250000;
//     ish_put(pos, neg);
// }

void ish_init() {
    /** Sets up the input stage peripheral (variant specific) */
    ish_setup_io();

    /** Initializes TCB0 in Frequency and Pulse Width measurement mode */
    tcb_init((void*)0);
    tcb_enable_interrupt();

    /** Connects Event user TCB0 (0) to Event generator Async channel 0 (0x3) */
    evsys_user_async_select(0, EVSYS_ASYNCUSER0_ASYNCCH0_gc);
}

void ish_enable(void) {
    evsys_gen_async_select(0, ish_get_event_channel());
    tcb_enable();
}

void ish_disable(void) {
    evsys_gen_async_disable(0);
    tcb_disable();
}
