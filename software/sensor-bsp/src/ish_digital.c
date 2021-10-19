#include "board/ish.h"
#include "board/ish_internal.h"

#include "hal/port.h"
#include "hal/tca.h"
#include "hal/tcb.h"
#include "hal/evsys.h"

#include <avr/io.h>

const uint8_t ish_pin = 7;
const uint8_t evsys_channel = 0;

const port_input_configuration ish_config = {
    .input_sense = PORT_INPUT_SENSE_INTDISABLE,
    .inverted = false,
    .pullup = false
};

void ish_init() {
    port_setup_input(ish_pin, &ish_config);

    ish_tcb_init();
}

void ish_enable(void) {
    evsys_gen_async_select(evsys_channel, EVSYS_ASYNCCH0_PORTA_PIN0_gc + ish_pin);
    tca_enable();
    tcb_enable();
}

void ish_disable(void) {
    evsys_gen_async_disable(evsys_channel);
}

uint8_t ish_get_event_channel(void) {
    return evsys_channel;
}

bool ish_get_state(void) {
    return port_read(ish_pin) == PORT_IO_STATE_HIGH ? true : false;
}
