#include "board/board.h"

#include "hal/sys.h"
#include "hal/clkctrl.h"
#include "hal/port.h"
#include "hal/portmux.h"

#include "board/pins.h"
#include "board/osh.h"
#include "board/ish.h"

#include <stddef.h>

const uint32_t MCLK_FREQUENCY = 16000000UL / 64UL;

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

void board_vcom_init(void) {
    portmux_alt_enable(PORTMUX_FUNC_USART0);
    port_setup_output(VCOM_TX_PIN, &vcom_tx_config);

    #if defined(BOARD_TYPE_XPLAINED)
    /**
     * On the Xplained development board the mEDBG debugger is connected to the target
     * with full duplex UART
     */
    port_setup_input(VCOM_RX_PIN, &vcom_rx_config);
    #endif
}

void board_user_led_init(void) {

}

void board_user_led_set(void) {

}

void board_user_led_clear(void) {

}

void board_user_led_toggle(void) {
    
}

void board_init() {
    board_clock_init();

    osh_init();

    ish_init();

    board_vcom_init();
}
