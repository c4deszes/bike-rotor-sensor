#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TCB_CLKSEL_CLKDIV1,
    TCB_CLKSEL_CLKDIV2,
    TCB_CLKSEL_CLKTCA
} tcb_clksel;

typedef enum {
    TCB_TIMER_MODE_INT,
    TCB_TIMER_MODE_TIMEOUT,
    TCB_TIMER_MODE_CAPTURE,
    TCB_TIMER_MODE_FREQ_CAPTURE,
    TCB_TIMER_MODE_PULSE_CAPTURE,
    TCB_TIMER_MODE_FREQ_PULSE_CAPTURE,
    TCB_TIMER_MODE_SINGLE,
    TCB_TIMER_MODE_PWM8
} tcb_timer_mode;

typedef struct {
    bool runstandby;
    bool sync;
    tcb_clksel clksel;
    bool input_filter;
    bool event_edge;
    bool capture_event_enable;
} tcb_configuration;

void tcb_init(const tcb_configuration* configuration);

void tcb_enable(void);

void tcb_disable(void);

void tcb_enable_interrupt();

void tcb_disable_interrupt(void);

void tcb_capture_handler(const uint16_t capture, const uint16_t count);

bool tcb_is_running(void);