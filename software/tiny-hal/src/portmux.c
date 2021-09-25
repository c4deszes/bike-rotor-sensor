#include "hal/portmux.h"

#include <stddef.h>
#include <avr/io.h>

typedef struct {
    uint8_t* reg;
    uint8_t mask;
} portmux_reg;

portmux_reg portmux_get_register(const portmux_func pin) {
    portmux_reg out = {.reg = NULL, .mask = 0};
    switch (pin) {
        case PORTMUX_FUNC_LUT0:   out.reg = &(PORTMUX.CTRLA); out.mask = PORTMUX_LUT0_bm; break;
        case PORTMUX_FUNC_LUT1:   out.reg = &(PORTMUX.CTRLA); out.mask = PORTMUX_LUT1_bm; break;
        case PORTMUX_FUNC_EVOUT0: out.reg = &(PORTMUX.CTRLA); out.mask = PORTMUX_EVOUT0_bm; break;
        case PORTMUX_FUNC_EVOUT1: out.reg = &(PORTMUX.CTRLA); out.mask = PORTMUX_EVOUT1_bm; break;
        case PORTMUX_FUNC_EVOUT2: out.reg = &(PORTMUX.CTRLA); out.mask = PORTMUX_EVOUT2_bm; break;
        case PORTMUX_FUNC_SPI0:   out.reg = &(PORTMUX.CTRLB); out.mask = PORTMUX_SPI0_bm; break;
        case PORTMUX_FUNC_USART0: out.reg = &(PORTMUX.CTRLB); out.mask = PORTMUX_USART0_bm; break;
        case PORTMUX_FUNC_TCA00:  out.reg = &(PORTMUX.CTRLC); out.mask = PORTMUX_TCA00_bm; break;
        case PORTMUX_FUNC_TCA01:  out.reg = &(PORTMUX.CTRLC); out.mask = PORTMUX_TCA01_bm; break;
        case PORTMUX_FUNC_TCA02:  out.reg = &(PORTMUX.CTRLC); out.mask = PORTMUX_TCA02_bm; break;
        case PORTMUX_FUNC_TCA03:  out.reg = &(PORTMUX.CTRLC); out.mask = PORTMUX_TCA03_bm; break;
        case PORTMUX_FUNC_TCB0:   out.reg = &(PORTMUX.CTRLD); out.mask = PORTMUX_TCB0_bm; break;
    }
    return out;
}

void portmux_alt_enable(const portmux_func pin) {
    portmux_reg setting = portmux_get_register(pin);
    *(setting.reg) |= setting.mask;
}

void portmux_alt_disable(const portmux_func pin) {
    portmux_reg setting = portmux_get_register(pin);
    *(setting.reg) &= ~(setting.mask);
}

