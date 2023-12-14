#include "hal/tcc.h"

#include <stdlib.h>
#include "atsamd21e18a.h"

static tcc_registers_t* get_peripheral(uint8_t timer) {
    switch (timer) {
        case TCC0: return TCC0_REGS;
    }
    // TODO: is this good?
    return NULL;
}

static IRQn_Type get_interrupt(uint8_t timer) {
    switch(timer) {
        case TCC0: return TCC0_IRQn;
    }
    return PERIPH_MAX_IRQn + 1;
}

void TCC_SetupTrigger(uint8_t timer, uint32_t period) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_CTRLA = TCC_CTRLA_PRESCALER_DIV1;
    peripheral->TCC_INTENSET = TCC_INTENSET_OVF_Msk;
    peripheral->TCC_INTFLAG = TCC_INTFLAG_Msk;
    peripheral->TCC_PER = period;

    NVIC_SetPriority(get_interrupt(timer), 10);
    NVIC_EnableIRQ(get_interrupt(timer));
}

void TCC_Enable(uint8_t timer) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_CTRLA |= TCC_CTRLA_ENABLE_Msk;
    while((peripheral->TCC_SYNCBUSY & TCC_SYNCBUSY_ENABLE_Msk) != 0);
}

void TCC_Reset(uint8_t timer) {
    tcc_registers_t* peripheral = get_peripheral(timer);

    peripheral->TCC_CTRLA |= TCC_CTRLA_SWRST_Msk;
    while((peripheral->TCC_SYNCBUSY & TCC_SYNCBUSY_SWRST_Msk) != 0);
}

static void _DummyInterruptHandler(void) {}

void TCC0_Interrupt(void) __attribute__((weak, alias("_DummyInterruptHandler")));

void TCC0_Handler(void) {
    TCC0_Interrupt();

    TCC0_REGS->TCC_INTFLAG = TCC_INTFLAG_Msk;
}
