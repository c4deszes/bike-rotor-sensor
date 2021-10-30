#include "hal/port.h"

#include <avr/io.h>

inline uint8_t port_get_input_sense_reg(const port_input_sense input_sense) {
    switch (input_sense) {
        case PORT_INPUT_SENSE_INTDISABLE: return PORT_ISC_INTDISABLE_gc;
        case PORT_INPUT_SENSE_BOTH: return PORT_ISC_BOTHEDGES_gc;
        case PORT_INPUT_SENSE_RISING: return PORT_ISC_RISING_gc;
        case PORT_INPUT_SENSE_FALLING: return PORT_ISC_FALLING_gc;
        case PORT_INPUT_SENSE_DISABLE: return PORT_ISC_INPUT_DISABLE_gc;
        case PORT_INPUT_SENSE_LEVEL: return PORT_ISC_LEVEL_gc;
        default: return PORT_ISC_INTDISABLE_gc;
    }
}

void port_setup_standby(const uint8_t pin) {
    PORTA.DIRCLR = _BV(pin);
    volatile uint8_t* pinctrl = &(PORTA.PIN0CTRL);
    pinctrl[pin] = port_get_input_sense_reg(PORT_INPUT_SENSE_DISABLE) | PORT_PULLUPEN_bm;
}

inline uint8_t port_get_input_pinctrl(const port_input_configuration* configuration) {
    uint8_t pinctrl = 0;
    if (configuration->inverted) {
        pinctrl |= PORT_INVEN_bm;
    }
    if (configuration->pullup) {
        pinctrl |= PORT_PULLUPEN_bm;
    }
    pinctrl |= port_get_input_sense_reg(configuration->input_sense);
    return pinctrl;
}

void port_setup_input(const uint8_t pin, const port_input_configuration* configuration) {
    PORTA.DIRCLR = _BV(pin);
    volatile uint8_t* pinctrl = &(PORTA.PIN0CTRL);
    pinctrl[pin] = port_get_input_pinctrl(configuration);
}

inline uint8_t port_get_output_pinctrl(const port_output_configuration* configuration) {
    uint8_t pinctrl = 0;
    if (configuration->inverted) {
        pinctrl |= PORT_INVEN_bm;
    }
    pinctrl |= port_get_input_sense_reg(configuration->input_sense);
    return pinctrl;
}

void port_setup_output(const uint8_t pin, const port_output_configuration* configuration) {
    PORTA.DIRSET = _BV(pin);
    volatile uint8_t* pinctrl = &(PORTA.PIN0CTRL);
    pinctrl[pin] = port_get_output_pinctrl(configuration);
}

port_io_state port_read(const uint8_t pin) {
    uint8_t input = PORTA.IN;
    if ((input & _BV(pin)) != 0) {
        return PORT_IO_STATE_HIGH;
    }
    else {
        return PORT_IO_STATE_LOW;
    }
}

void port_write(const uint8_t pin, const port_io_state state) {
    if (state == PORT_IO_STATE_LOW) {
        PORTA.OUTCLR = _BV(pin);
    }
    else if (state == PORT_IO_STATE_HIGH) {
        PORTA.OUTSET = _BV(pin);
    }
}

void port_toggle(const uint8_t pin) {
    PORTA.OUTTGL = _BV(pin);
}
