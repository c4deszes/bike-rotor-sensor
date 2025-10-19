#include "hal/pm.h"
#include "atsamd21e18a.h"

pm_rcause PM_ResetCause(void) {
    return (pm_rcause) PM_REGS->PM_RCAUSE;
}

void PM_SelectCpuDiv(uint8_t div) {
    PM_REGS->PM_CPUSEL = div;
}

void PM_SelectBusDiv(uint8_t apba_sel, uint8_t apbb_sel, uint8_t apbc_sel) {
    PM_REGS->PM_APBASEL = apba_sel;
    PM_REGS->PM_APBBSEL = apbb_sel;
    PM_REGS->PM_APBCSEL = apbc_sel;
}

void PM_IdleModeEnter(void) {
    /* Configure Idle Sleep mode */
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
    PM_REGS->PM_SLEEP = PM_SLEEP_IDLE(0);
    /* Wait for interrupt instruction execution */
    __WFI();
}

void PM_StandbyModeEnter(void) {
    /* Configure Standby Sleep */
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    /* Wait for interrupt instruction execution */
    __WFI();
}
