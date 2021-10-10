#include "board/board.h"

#include "hal/sys.h"
#include "hal/clkctrl.h"
#include "hal/port.h"
#include "hal/portmux.h"
#include "board/osh.h"
#include "board/ish.h"
#include <stddef.h>

const uint32_t MCLK_FREQUENCY = 16000000UL;

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

const uint8_t vcom_tx_pin = 1;
const port_output_configuration vcom_tx_config = {
    .input_sense = PORT_INPUT_SENSE_INTDISABLE,
    .inverted = false
};

const uint8_t vcom_rx_pin = 2;
const port_input_configuration vcom_rx_config = {
    .input_sense = PORT_INPUT_SENSE_INTDISABLE,
    .inverted = false,
    .pullup = true
};

void vcom_init(void) {
    portmux_alt_enable(PORTMUX_FUNC_USART0);
    port_setup_output(vcom_tx_pin, &vcom_tx_config);
    port_setup_input(vcom_rx_pin, &vcom_rx_config);
}

void board_init() {
    sys_init(MCLK_FREQUENCY);

    clock_init();

    osh_init();

    ish_init(NULL);

    vcom_init();
}
