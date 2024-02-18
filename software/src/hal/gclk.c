#include "hal/gclk.h"

#include "atsamd21e18a.h"

void GCLK_ConfigureGenerator(uint8_t generator, uint8_t source, uint16_t division) {
    GCLK_REGS->GCLK_GENCTRL = GCLK_GENCTRL_SRC(source) | GCLK_GENCTRL_GENEN_Msk | GCLK_GENCTRL_ID(generator);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk);

    GCLK_REGS->GCLK_GENDIV = GCLK_GENDIV_DIV(division) | GCLK_GENDIV_ID(generator);

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk);
}

void GCLK_SelectGenerator(uint8_t id, uint8_t generator) {
    GCLK_REGS->GCLK_CLKCTRL = GCLK_CLKCTRL_ID(id) | GCLK_CLKCTRL_GEN(generator)  | GCLK_CLKCTRL_CLKEN_Msk;

    while((GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk) == GCLK_STATUS_SYNCBUSY_Msk);
}

void GCLK_Reset(void) {
    GCLK_REGS->GCLK_CTRL = GCLK_CTRL_SWRST_Msk;

    while ((GCLK_REGS->GCLK_CTRL & GCLK_CTRL_SWRST_Msk) && (GCLK_REGS->GCLK_STATUS & GCLK_STATUS_SYNCBUSY_Msk));
}
