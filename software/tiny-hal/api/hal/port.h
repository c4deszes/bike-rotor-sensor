#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    PORT_IO_STATE_LOW,
    PORT_IO_STATE_HIGH
} port_io_state;

typedef enum {
    // TODO: rename INTDISABLE and DISABLE
    PORT_INPUT_SENSE_INTDISABLE,
    PORT_INPUT_SENSE_BOTH,
    PORT_INPUT_SENSE_RISING,
    PORT_INPUT_SENSE_FALLING,
    PORT_INPUT_SENSE_DISABLE,
    PORT_INPUT_SENSE_LEVEL
} port_input_sense;

typedef struct {
    bool inverted;
    bool pullup;
    port_input_sense input_sense;
} port_input_configuration;

typedef struct {
    bool inverted;
    port_input_sense input_sense;
} port_output_configuration;

void port_setup_standby(const uint8_t pin);

void port_setup_input(const uint8_t pin, const port_input_configuration* configuration);

void port_setup_output(const uint8_t pin, const port_output_configuration* configuration);

port_io_state port_read(const uint8_t pin);

void port_write(const uint8_t pin, const port_io_state state);

void port_toggle(const uint8_t pin);
