#pragma once

typedef enum {
    PORTMUX_FUNC_LUT0,
    PORTMUX_FUNC_LUT1,
    PORTMUX_FUNC_EVOUT0,
    PORTMUX_FUNC_EVOUT1,
    PORTMUX_FUNC_EVOUT2,
    PORTMUX_FUNC_SPI0,
    PORTMUX_FUNC_USART0,
    PORTMUX_FUNC_TCA00,
    PORTMUX_FUNC_TCA01,
    PORTMUX_FUNC_TCA02,
    PORTMUX_FUNC_TCA03,
    PORTMUX_FUNC_TCB0
} portmux_func;

/**
 * @brief Enables the alternate pin location for the given pin
 * 
 * @param pin 
 */
void portmux_alt_enable(const portmux_func pin);

/**
 * @brief Disables the alternate pin location for the given pin
 * 
 * @param pin 
 */
void portmux_alt_disable(const portmux_func pin);
