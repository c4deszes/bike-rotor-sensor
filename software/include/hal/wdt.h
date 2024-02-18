/**
 * @file watchdog.h
 * @author Balazs Eszes
 * @brief Watchdog interface
 */
#ifndef HAL_WDT_H_
#define HAL_WDT_H_

#include <stdbool.h>

/**
 * Watchdog timeout values
 */
typedef enum {
    WDT_TIMEOUT_CYC8 = 0x00,
    WDT_TIMEOUT_CYC16 = 0x01,
    WDT_TIMEOUT_CYC32 = 0x02,
    WDT_TIMEOUT_CYC64 = 0x03,
    WDT_TIMEOUT_CYC128 = 0x04,
    WDT_TIMEOUT_CYC256 = 0x05,
    WDT_TIMEOUT_CYC512 = 0x06,
    WDT_TIMEOUT_CYC1024 = 0x07,
    WDT_TIMEOUT_CYC2048 = 0x08,
    WDT_TIMEOUT_CYC4096 = 0x09,
    WDT_TIMEOUT_CYC8192 = 0x0A,
    WDT_TIMEOUT_CYC16384 = 0x0B
} wdt_timeout;

/**
 * Watchdog Normal mode configuration
 */
typedef struct {
    wdt_timeout period;             /**< Watchdog period */
} wdt_normal_configuration;

/**
 * @brief Initializes the Watchdog in normal mode
 * 
 * @param wdt_conf Pointer to Watchdog window configuration
 */
void WDT_InitializeNormal(const wdt_normal_configuration* wdt_conf);

/**
 * @brief Disables the Watchdog peripheral
 */
void WDT_Disable(void);

/**
 * @brief Acknowledges the Watchdog
 * 
 * In NORMAL mode this function can be called anytime
 * 
 * In WINDOWED mode this function can only be called within the Watchdog timeout
 * window, otherwise it will reset the microcontroller
 */
void WDT_Acknowledge(void);

/**
 * @brief Forces the microcontroller to self reset by writing the incorrect
 * value into the Watchdog clear register
 */
void WDT_SelfReset(void);

#endif  // HAL_WDT_H_
