#pragma once

#include <stdbool.h>

typedef enum {
    ADC_RESOLUTION_10BIT,
    ADC_RESOLUTION_8BIT
} adc_resolution;

typedef enum {
    ADC_SAMPLING_NONE,
    ADC_SAMPLING_ACC2,
    ADC_SAMPLING_ACC4,
    ADC_SAMPLING_ACC8,
    ADC_SAMPLING_ACC16,
    ADC_SAMPLING_ACC32,
    ADC_SAMPLING_ACC64
} adc_sampling;

typedef struct {
    bool runstandby;
    adc_resolution resolution;
} adc_configuration;

void adc_init(adc_configuration configuration);

typedef enum {
    ADC_MUXPOS_AIN0,
    ADC_MUXPOS_AIN1,
    ADC_MUXPOS_AIN2,
    ADC_MUXPOS_AIN3,
    ADC_MUXPOS_AIN4,
    ADC_MUXPOS_AIN5,
    ADC_MUXPOS_AIN6,
    ADC_MUXPOS_AIN7,
    ADC_MUXPOS_AIN8,
    ADC_MUXPOS_AIN9,
    ADC_MUXPOS_AIN10,
    ADC_MUXPOS_AIN11,
    ADC_MUXPOS_INTREF,
    ADC_MUXPOS_TEMPSENSE,
    ADC_MUXPOS_GND,
} adc_muxpos;

void adc_setmux(adc_muxpos muxpos);

void adc_start_conversion(void);
