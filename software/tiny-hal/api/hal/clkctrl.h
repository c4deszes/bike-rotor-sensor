#pragma once

typedef enum {
    CLKCTRL_CLOCK_SELECT_OSC20M = 0x0,
    CLKCTRL_CLOCK_SELECT_OSCULP32K = 0x1,
    CLKCTRL_CLOCK_SELECT_EXTCLK = 0x3
} clkctrl_clock_select;

typedef enum {
    CLKCTRL_PRESCALER_DIV2 = 0x0,
    CLKCTRL_PRESCALER_DIV4 = 0x1,
    CLKCTRL_PRESCALER_DIV8 = 0x2,
    CLKCTRL_PRESCALER_DIV16 = 0x3,
    CLKCTRL_PRESCALER_DIV32 = 0x4,
    CLKCTRL_PRESCALER_DIV64 = 0x5,
    CLKCTRL_PRESCALER_DIV6 = 0x8,
    CLKCTRL_PRESCALER_DIV10 = 0x9,
    CLKCTRL_PRESCALER_DIV12 = 0xA,
    CLKCTRL_PRESCALER_DIV24 = 0xB,
    CLKCTRL_PRESCALER_DIV48 = 0xC,
    CLKCTRL_PRESCALER_NODIV = 0xF
} clkctrl_prescaler;

/**
 * @brief Sets the clock source for the core
 * 
 * 
 * 
 * @param clock 
 */
void clkctrl_set_clock(const clkctrl_clock_select clock);

void clkctrl_set_prescaler(const clkctrl_prescaler prescaler);

void clkctrl_lock();
