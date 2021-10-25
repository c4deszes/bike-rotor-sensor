#include "board/ish.h"
#include "board/ish_internal.h"

#include "hal/port.h"
#include "board/pins.h"

#include <avr/io.h>

const port_input_configuration ish_config = {
    .input_sense = PORT_INPUT_SENSE_INTDISABLE,
    .inverted = false,
    .pullup = false
};

void ish_setup_io() {
    port_setup_input(ISH_INPUT_PIN, &ish_config);
}

uint8_t ish_get_event_channel(void) {
    return EVSYS_ASYNCCH0_PORTA_PIN0_gc + ISH_INPUT_PIN;
}

bool ish_get_state(void) {
    return port_read(ISH_INPUT_PIN) == PORT_IO_STATE_HIGH ? true : false;
}
