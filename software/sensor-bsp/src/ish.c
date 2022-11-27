#include "board/ish.h"
#include "board/ish_internal.h"

#include "hal/sys.h"
#include "hal/evsys.h"
#include "hal/tca.h"
#include "hal/tcb.h"

#define BUFFER_SIZE 10

static bool overflow = false;
static uint16_t pos_buffer[BUFFER_SIZE];
static uint16_t neg_buffer[BUFFER_SIZE];
static uint8_t head = 0;
static uint8_t tail = 0;

static const tca_configuration tca_config = {
    .clksel = TCA_CLKSEL_CLKDIV256
};

void ish_put(uint16_t pos, uint16_t neg) {
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
        ish_data data = {0xFFFF, 0xFFFF};
        return data;
    }
    ish_data data = {pos_buffer[tail], neg_buffer[tail]};
    tail = (tail + 1) % BUFFER_SIZE;
    return data;
}

static volatile uint8_t pulses = 0;
static volatile bool tca_oveflow = false;

void tca_overflow_handler(void) {
    if(pulses > 0) {
        pulses = 0;
        tca_oveflow = false;
    }
    else {
        tca_oveflow = true;
        ish_put(0, 0xFFFF);
    }
}

void tcb_capture_handler(const uint16_t capture, const uint16_t count) {
    if(tca_oveflow) {
        pulses++;
        ish_put(0, 0xFFFF);
    }
    else {
        pulses++;
        ish_put(capture, count);
    }
}

void ish_init() {
    /** Sets up the input stage peripheral (variant specific) */
    ish_setup_io();

    /** Initializes TCB0 in Frequency and Pulse Width measurement mode */
    tcb_init((void*)0);
    tcb_enable_interrupt();

    tca_init(&tca_config);
    tca_set_period(0xFFFF);
    tca_enable_overflow_interrupt();

    /** Connects Event user TCB0 (0) to Event generator Async channel 0 (0x3) */
    //evsys_user_async_select(0, EVSYS_ASYNCUSER0_ASYNCCH0_gc);
    evsys_user_async_select(0, 0x3);
}

void ish_enable(void) {
    evsys_gen_async_select(0, ish_get_event_channel());
    tca_enable();
    tcb_enable();
}

void ish_disable(void) {
    evsys_gen_async_disable(0);
    tca_disable();
    tcb_disable();
}
