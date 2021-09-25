#pragma once

typedef enum {
    VREF_VALUE_0V55 = 0,
    VREF_VALUE_1V1 = 1,
    VREF_VALUE_2V5 = 2,
    VREF_VALUE_4V3 = 3,
    VREF_VALUE_1V5 = 4
} vref_value;

void vref_select(const vref_value adc0, const vref_value ac0);

void vref_select_adc0(const vref_value value);

void vref_select_ac0(const vref_value value);
