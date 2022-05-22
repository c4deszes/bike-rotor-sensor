#include "board/board.h"

#include "hal/sys.h"
#include "hal/clkctrl.h"
#include "hal/port.h"
#include "hal/portmux.h"

#include "board/pins.h"
#include "board/osh.h"
#include "board/ish.h"

#include <stddef.h>

const uint32_t MCLK_FREQUENCY = 20000000UL / 64UL;

/**
 * @brief Initializes the clock settings
 * 
 * @note Initial settings depend on the fuse settings
 */
void board_clock_init(void) {
    sys_init(MCLK_FREQUENCY);
    clkctrl_set_clock(CLKCTRL_CLOCK_SELECT_OSC20M);
    clkctrl_set_prescaler(CLKCTRL_PRESCALER_DIV64);
    clkctrl_lock();
}

const port_output_configuration vcom_tx_config = {
    .input_sense = PORT_INPUT_SENSE_INTDISABLE,
    .inverted = false
};

const port_input_configuration vcom_rx_config = {
    .input_sense = PORT_INPUT_SENSE_INTDISABLE,
    .inverted = false,
    .pullup = true
};

const port_output_configuration vcom_cs_config = {
    .input_sense = PORT_INPUT_SENSE_DISABLE,
    .inverted = false
};

void board_vcom_init(void) {
    portmux_alt_enable(PORTMUX_FUNC_USART0);
    port_setup_output(VCOM_TX_PIN, &vcom_tx_config);
    port_setup_input(VCOM_RX_PIN, &vcom_rx_config);
    port_setup_output(VCOM_CS_PIN, &vcom_cs_config);
}

void board_vcom_select(void) {
    port_write(VCOM_CS_PIN, PORT_IO_STATE_HIGH);
}

void board_goto_sleep(void) {
    port_write(VCOM_CS_PIN, PORT_IO_STATE_LOW);
}

#if defined(BOARD_TYPE_XPLAINED)
#include <avr/io.h>

void board_user_led_init(void) {
    PORTB.OUTSET = _BV(5);
    PORTB.DIRSET = _BV(5);
}

void board_user_led_on(void) {
    PORTB.OUTCLR = _BV(5);
}

void board_user_led_off(void) {
    PORTB.OUTSET = _BV(5);
}

void board_user_led_toggle(void) {
    PORTB.OUTTGL = _BV(5);
}
#endif

void board_init() {
    board_clock_init();

    osh_init();

    ish_init();

    board_vcom_init();

    #if defined(BOARD_TYPE_XPLAINED)
    board_user_led_init();
    #endif
}
