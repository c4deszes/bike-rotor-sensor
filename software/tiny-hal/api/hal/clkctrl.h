#pragma once

typedef enum {
    CLKCTRL_CLOCK_SELECT_OSC20M = 0x0,      /** 20MHz internal clock */
    CLKCTRL_CLOCK_SELECT_OSCULP32K = 0x1,   /** Ultra low power 32K oscillator */
    CLKCTRL_CLOCK_SELECT_EXTCLK = 0x3       /** External clock */
} clkctrl_clock_select;

typedef enum {
    CLKCTRL_PRESCALER_DIV2 = 0x0,           /** Main clock divided by 2 */
    CLKCTRL_PRESCALER_DIV4 = 0x1,           /** Main clock divided by 4 */
    CLKCTRL_PRESCALER_DIV8 = 0x2,           /** Main clock divided by 8 */
    CLKCTRL_PRESCALER_DIV16 = 0x3,          /** Main clock divided by 16 */
    CLKCTRL_PRESCALER_DIV32 = 0x4,          /** Main clock divided by 32 */
    CLKCTRL_PRESCALER_DIV64 = 0x5,          /** Main clock divided by 64 */
    CLKCTRL_PRESCALER_DIV6 = 0x8,           /** Main clock divided by 6 */
    CLKCTRL_PRESCALER_DIV10 = 0x9,          /** Main clock divided by 10 */
    CLKCTRL_PRESCALER_DIV12 = 0xA,          /** Main clock divided by 12 */
    CLKCTRL_PRESCALER_DIV24 = 0xB,          /** Main clock divided by 24 */
    CLKCTRL_PRESCALER_DIV48 = 0xC,          /** Main clock divided by 48 */
    CLKCTRL_PRESCALER_NODIV = 0xF           /** Main clock divided by 1 */
} clkctrl_prescaler;

/**
 * @brief Sets the clock source for the core (CLK_MAIN)
 * @note The default clock selection is based on fuse settings, therefore calling this function
 * may not be necessary
 * 
 * @param clock Main clock source
 */
void clkctrl_set_clock(const clkctrl_clock_select clock);

/**
 * @brief Sets the main clock prescaler for peripherals (CLK_PER)
 * @note The default prescaler selection is based on fuse settings, therefore calling this function
 * may not be necessary
 * 
 * @param prescaler Prescaler factor
 */
void clkctrl_set_prescaler(const clkctrl_prescaler prescaler);

/**
 * @brief Locks the clock settings
 * @note The clock settings may be locked by default depending on fuse settings
 */
void clkctrl_lock(void);
