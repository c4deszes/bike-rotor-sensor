#include "hal/adc.h"

#include <avr/io.h>

void adc_init(adc_configuration* conf) {
    ADC0.CTRLA |= ADC_RESSEL_bm;    /** 8bit mode */
    ADC0.CTRLB = conf->sampling;
    //ADC0.CTRLC = ADC_SAMPCAP_bm | (ADC_REFSEL_VDDREF_gc) | ADC_PRESC_DIV16_gc;
    ADC0.CTRLC = ADC_SAMPCAP_bm | (ADC_REFSEL_INTREF_gc) | ADC_PRESC_DIV16_gc;
    ADC0.CTRLD = ADC_INITDLY_DLY64_gc;
    ADC0.SAMPCTRL = 8;

    /** Enable ADC */
    ADC0.CTRLA |= ADC_ENABLE_bm;
}

void adc_setmux(adc_muxpos muxpos) {
    ADC0.MUXPOS = muxpos;
}

void adc_start(void) {
    ADC0.COMMAND = ADC_STCONV_bm;
}

uint8_t adc_sync_read(void) {
    while((ADC0.INTFLAGS & ADC_RESRDY_bm) == 0);

    return ADC0.RESL;
}
