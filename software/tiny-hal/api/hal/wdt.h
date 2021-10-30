#pragma once

typedef enum {
    WDT_PERIOD_OFF = 0,
    WDT_PERIOD_8CLK = 1,
    WDT_PERIOD_16CLK = 2,
    WDT_PERIOD_32CLK = 3,
    WDT_PERIOD_64CLK = 4,
    WDT_PERIOD_128CLK = 5,
    WDT_PERIOD_256CLK = 6,
    WDT_PERIOD_512CLK = 7,
    WDT_PERIOD_1024CLK = 8,
    WDT_PERIOD_2048CLK = 9,
    WDT_PERIOD_4096CLK = 10,
    WDT_PERIOD_8192CLK = 11
} wdt_period;

/**
 * @brief Initializes the watchdog in normal mode
 * 
 * @param period Watchdog period
 */
void wdt_init_normal(const wdt_period period);

/**
 * @brief Initializes the watchdog in windowed mode
 * 
 * @param open Open window period
 * @param closed Closed window period
 */
void wdt_init_window(const wdt_period open, const wdt_period closed);

/**
 * @brief Acknowledges the watchdog
 */
void wdt_acknowledge();
