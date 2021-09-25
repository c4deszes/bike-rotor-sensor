#include "hal/vref.h"

#include <avr/io.h>

void vref_select(const vref_value adc0, const vref_value ac0) {
    VREF.CTRLA = (adc0 << VREF_ADC0REFSEL_gp)
                 | (ac0 << VREF_DAC0REFSEL_gp);
}

void vref_select_adc0(const vref_value value) {
    VREF.CTRLA = (VREF.CTRLA & ~VREF_ADC0REFSEL_gm)
                 | (value << VREF_ADC0REFSEL_gp);
}

void vref_select_ac0(const vref_value value) {
    VREF.CTRLA = (VREF.CTRLA & ~VREF_DAC0REFSEL_gm)
                 | (value << VREF_DAC0REFSEL_gp);
}
