#pragma once

#include <stdint.h>

typedef enum {
    TCA_CLKSEL_CLKDIV1 = 0,
    TCA_CLKSEL_CLKDIV2 = 1,
    TCA_CLKSEL_CLKDIV4 = 2,
    TCA_CLKSEL_CLKDIV8 = 3,
    TCA_CLKSEL_CLKDIV16 = 4,
    TCA_CLKSEL_CLKDIV64 = 5,
    TCA_CLKSEL_CLKDIV256 = 6,
    TCA_CLKSEL_CLKDIV1024 = 7
} tca_clksel;

typedef struct {
    tca_clksel clksel;
} tca_configuration;

typedef void (*tca_ovf_handler)(void);

void tca_init(const tca_configuration* configuration);

void tca_enable(void);

void tca_disable(void);

void tca_set_period(uint16_t period);

void tca_enable_overflow_interrupt(tca_ovf_handler handler);

void tca_disable_overflow_interrupt(void);
