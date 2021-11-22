#pragma once

/**
 * @brief Initializes the board peripherals and clock system
 */
void board_init(void);

/**
 * @brief Initializes the clock
 */
void board_clock_init(void);

/**
 * @brief Initializes the UART IO
 */
void board_vcom_init(void);

void board_user_led_init(void);

void board_user_led_set(void);

void board_user_led_clear(void);

void board_user_led_toggle(void);
