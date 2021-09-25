#include "hal/adc.h"

#include <avr/io.h>

void adc_setmux(adc_muxpos muxpos) {
    ADC0.MUXPOS = muxpos;
}

void adc_start_conversion(void) {
    ADC0.COMMAND = ADC_STCONV_bm;
}
