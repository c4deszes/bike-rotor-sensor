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

/**
 * @brief 
 */
void board_vcom_select(void);

/**
 * @brief 
 * 
 */
void board_goto_sleep(void);

#ifdef BOARD_TYPE_XPLAINED
/**
 * @brief Initializes the user LED
 */
void board_user_led_init(void);

/**
 * @brief Turns on the user LED
 */
void board_user_led_on(void);

/**
 * @brief Turns off the user LED
 */
void board_user_led_off(void);

/**
 * @brief Toggles the user LED
 */
void board_user_led_toggle(void);

#endif
