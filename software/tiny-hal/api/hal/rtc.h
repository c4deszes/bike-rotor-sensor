#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    RTC_PRESCALER_DIV1 = 0,
    RTC_PRESCALER_DIV2 = 1,
    RTC_PRESCALER_DIV4 = 2,
    RTC_PRESCALER_DIV8 = 3,
    RTC_PRESCALER_DIV16 = 4,
    RTC_PRESCALER_DIV32 = 5,
    RTC_PRESCALER_DIV64 = 6,
    RTC_PRESCALER_DIV128 = 7,
    RTC_PRESCALER_DIV256 = 8,
    RTC_PRESCALER_DIV512 = 9,
    RTC_PRESCALER_DIV1024 = 10,
    RTC_PRESCALER_DIV2048 = 11,
    RTC_PRESCALER_DIV4096 = 12,
    RTC_PRESCALER_DIV8192 = 13,
    RTC_PRESCALER_DIV16384 = 14,
    RTC_PRESCALER_DIV32768 = 15
} rtc_prescaler;

typedef enum {
    RTC_CLKSEL_INT32K = 0,
    RTC_CLKSEL_INT1K = 1,
    RTC_CLKSEL_EXTCLK = 3
} rtc_clksel;

typedef struct {
    bool standby;
    rtc_prescaler prescaler;
    rtc_clksel clksel;
} rtc_configuration;

/**
 * @brief Initializes the Real-Time Clock with the given settings
 * 
 * @param configuration 
 */
void rtc_init(const rtc_configuration* configuration);

/**
 * @brief Enables the RTC peripheral
 * 
 */
void rtc_enable(void);

/**
 * @brief Disables the RTC peripheral
 * 
 */
void rtc_disable(void);

void rtc_set_period(uint16_t period);

void rtc_set_compare(uint16_t compare);

typedef enum {
    RTC_PIT_PERIOD_OFF = 0,
    RTC_PIT_PERIOD_CYC4 = 1,
    RTC_PIT_PERIOD_CYC8 = 2,
    RTC_PIT_PERIOD_CYC16 = 3,
    RTC_PIT_PERIOD_CYC32 = 4,
    RTC_PIT_PERIOD_CYC64 = 5,
    RTC_PIT_PERIOD_CYC128 = 6,
    RTC_PIT_PERIOD_CYC256 = 7,
    RTC_PIT_PERIOD_CYC512 = 8,
    RTC_PIT_PERIOD_CYC1024 = 9,
    RTC_PIT_PERIOD_CYC2048 = 10,
    RTC_PIT_PERIOD_CYC4096 = 11,
    RTC_PIT_PERIOD_CYC8192 = 12,
    RTC_PIT_PERIOD_CYC16384 = 13,
    RTC_PIT_PERIOD_CYC32768 = 14
} rtc_pit_period;

void rtc_set_pit_period(rtc_pit_period period);

void rtc_enable_pit_interrupt(void);

void rtc_pit_handler(void);
