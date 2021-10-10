#pragma once

#include <stdbool.h>

typedef void (*ac_compare_handler)(bool state);

typedef enum {
    AC_INTERRUPT_MODE_BOTH = 0,
    AC_INTERRUPT_MODE_NEGATIVE = 2,
    AC_INTERRUPT_MODE_POSITIVE = 3
} ac_interrupt_mode;

typedef enum {
    AC_HYSTERESIS_MODE_OFF = 0,
    AC_HYSTERESIS_MODE_10MV = 1,
    AC_HYSTERESIS_MODE_25MV = 2,
    AC_HYSTERESIS_MODE_50MV = 3
} ac_hysteresis_mode;

typedef struct {
    bool runstandy;
    bool output_enable;
    ac_interrupt_mode intmode;
    ac_hysteresis_mode hysmode;
} ac_configuration;

typedef enum {
    AC_MUX_POSITIVE_P0 = 0,         /**< AINP0 as positive input */
    AC_MUX_POSITIVE_P1 = 1          /**< AINP1 as positive input */
} ac_mux_positive;

typedef enum {
    AC_MUX_NEGATIVE_N0 = 0,         /**< AINN0 as negative input */
    AC_MUX_NEGATIVE_N1 = 1,         /**< AINN1 as negative input */
    AC_MUX_NEGATIVE_VREF = 2,       /**< Voltage reference as negative input */
} ac_mux_negative;

/**
 * @brief Initializes the Analog Comparator
 * @note Peripheral must be disabled before changing configuration
 * 
 * @param configuration Analog Comparator settings
 */
void ac_init(const ac_configuration* configuration);

/**
 * @brief Enables the Analog Comparator
 */
void ac_enable(void);

/**
 * @brief Disables the Analog Comparator
 */
void ac_disable(void);

/**
 * @brief Selects the inputs of the Analog Comparator
 * 
 * @param invert When enabled the output of the peripheral will be inverted
 * @param positive Positive analog comparator input
 * @param negative Negative analog comparator input
 */
void ac_setmux(const bool invert, const ac_mux_positive positive, const ac_mux_negative negative);

/**
 * @brief Enables the Analog Comparator interrupt
 * 
 * @param handler Interrupt handler
 */
void ac_interrupt_enable(ac_compare_handler handler);

/**
 * @brief Disables the Analog Comparator interrupt
 */
void ac_interrupt_disable(void);

/**
 * @brief Returns the current state of the Analog Comparator, the returned state
 * is inverted if the peripheral is configured to be inverted
 * 
 * @return true If the positive input is higher than the negative input
 * @return false If the negative input is higher than the positive input
 */
bool ac_get_state();
