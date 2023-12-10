#include "hal/adc.h"

#include "atsamd21e18a.h"

static __inline__ void syncADC() __attribute__((always_inline, unused));
static void syncADC() {
    ADC_REGS->ADC_STATUS;
  //    while (ADC_REGS->STATUS.bit.SYNCBUSY == 1);
}

void ADC_Init(void) {
    //uint32_t bias = (*((uint32_t *) ADC_FUSES_BIASCAL_ADDR) & ADC_FUSES_BIASCAL_Msk) >> ADC_FUSES_BIASCAL_Pos;

    // ADC Linearity bits 4:0
    //uint32_t linearity = (*((uint32_t *) ADC_FUSES_LINEARITY_0_ADDR) & ADC_FUSES_LINEARITY_0_Msk) >> ADC_FUSES_LINEARITY_0_Pos;

    // ADC Linearity bits 7:5
    //linearity |= ((*((uint32_t *) ADC_FUSES_LINEARITY_1_ADDR) & ADC_FUSES_LINEARITY_1_Msk) >> ADC_FUSES_LINEARITY_1_Pos) << 5;

    //ADC_REGS->ADC_CALIB = ADC_CALIB_BIAS_CAL(bias) | ADC_CALIB_LINEARITY_CAL(linearity);

    // for now setting up in freerunning mode would be good enough
    // we need two channels though, 1000Hz polling is the max we need

    ADC_REGS->ADC_INPUTCTRL = ADC_INPUTCTRL_MUXPOS(0);
    syncADC();
    ADC_REGS->ADC_CTRLA = ADC_CTRLA_ENABLE_Msk;
    syncADC();
    ADC_REGS->ADC_SWTRIG = ADC_SWTRIG_START_Msk;

    while (ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk == 0);

    ADC_REGS->ADC_INTENCLR = ADC_INTENCLR_Msk;

    while (ADC_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk == 0);

    uint16_t value = ADC_REGS->ADC_RESULT;

    ADC_REGS->ADC_INTENCLR = ADC_INTENCLR_Msk;
}

void ADC_Setup(void) {
    // 1. reset
    // 2. prescaler selection (512 @ 1MHz -> 1.953 ksps)
    ADC_REGS->ADC_REFCTRL = ADC_REFCTRL_REFSEL_INTVCC0_Val | ADC_REFCTRL_REFCOMP_Msk;
    // 3. select resolution 8,10,12 bit CTRLB_RESSEL
    ADC_REGS->ADC_CTRLB = ADC_CTRLB_PRESCALER_DIV512 | ADC_CTRLB_RESSEL_8BIT | ADC_CTRLB_FREERUN_Msk;
    // 4. select sample number 1-1024 and averaging 1-1024
}

void ADC_Reset(void) {
    ADC_REGS->ADC_CTRLA = ADC_CTRLA_SWRST_Msk;
    syncADC();
}
