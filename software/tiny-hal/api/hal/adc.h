#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    ADC_RESOLUTION_10BIT,
    ADC_RESOLUTION_8BIT
} adc_resolution;

typedef enum {
    ADC_SAMPLING_NONE = 0x01,
    ADC_SAMPLING_ACC2 = 0x01,
    ADC_SAMPLING_ACC4 = 0x02,
    ADC_SAMPLING_ACC8 = 0x03,
    ADC_SAMPLING_ACC16 = 0x04,
    ADC_SAMPLING_ACC32 = 0x05,
    ADC_SAMPLING_ACC64 = 0x06
} adc_sampling;

typedef struct {
    bool runstandby;
    adc_resolution resolution;
    adc_sampling sampling;
} adc_configuration;

void adc_init(adc_configuration* configuration);

typedef enum {
    ADC_MUXPOS_AIN0 = 0x00,
    ADC_MUXPOS_AIN1 = 0x01,
    ADC_MUXPOS_AIN2 = 0x02,
    ADC_MUXPOS_AIN3 = 0x03,
    ADC_MUXPOS_AIN4 = 0x04,
    ADC_MUXPOS_AIN5 = 0x05,
    ADC_MUXPOS_AIN6 = 0x06,
    ADC_MUXPOS_AIN7 = 0x07,
    ADC_MUXPOS_AIN8 = 0x08,
    ADC_MUXPOS_AIN9 = 0x09,
    ADC_MUXPOS_AIN10 = 0x0A,
    ADC_MUXPOS_AIN11 = 0x0B,
    ADC_MUXPOS_INTREF = 0x1D,
    ADC_MUXPOS_TEMPSENSE = 0x1E,
    ADC_MUXPOS_GND = 0x1F,
} adc_muxpos;

void adc_setmux(adc_muxpos muxpos);

void adc_start(void);

uint8_t adc_sync_read(void);
