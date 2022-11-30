#include "board/osh.h"

#include "hal/port.h"
#include "hal/adc.h"
#include "board/pins.h"

const port_output_configuration osh_config = {
    .input_sense = PORT_INPUT_SENSE_INTDISABLE,
    .inverted = false
};

void osh_init(void) {
    port_setup_output(OSH_OUTPUT_PIN, &osh_config);
}

void osh_output_on(void) {
    port_write(OSH_OUTPUT_PIN, PORT_IO_STATE_HIGH);
}

void osh_output_off(void) {
    port_write(OSH_OUTPUT_PIN, PORT_IO_STATE_LOW);
}
