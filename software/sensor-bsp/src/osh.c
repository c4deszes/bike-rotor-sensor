#include "board/osh.h"

#include "hal/port.h"
#include "hal/adc.h"

const uint8_t osh_pin = 2;

const port_output_configuration osh_config = {
    .input_sense = PORT_INPUT_SENSE_INTDISABLE,
    .inverted = false
};

void osh_init(void) {
    port_setup_output(osh_pin, &osh_config);
}

void osh_output_on(void) {
    port_write(osh_pin, PORT_IO_STATE_HIGH);
}

void osh_output_off(void) {
    port_write(osh_pin, PORT_IO_STATE_LOW);
}

uint16_t osh_get_current(void) {
    // TODO: implement current sensing
    return 0;
}
