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
void clock_init(void) {
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

void vcom_init(void) {
    portmux_alt_enable(PORTMUX_FUNC_USART0);
    port_setup_output(VCOM_TX_PIN, &vcom_tx_config);
    #if defined(BOARD_TYPE_XPLAINED)
    port_setup_input(VCOM_RX_PIN, &vcom_rx_config);
    #endif
}

void board_init() {
    sys_init(MCLK_FREQUENCY);

    clock_init();

    osh_init();

    ish_init();

    vcom_init();
}
