#include "hal/wdt.h"

#include "atsamd21e18a.h"

void WDT_InitializeNormal(const wdt_normal_configuration* wdt_conf) {
    WDT_REGS->WDT_CONFIG = wdt_conf->period;
    WDT_REGS->WDT_CTRL = WDT_CTRL_ENABLE_Msk;

    while((WDT_REGS->WDT_STATUS & WDT_STATUS_SYNCBUSY_Msk) == WDT_STATUS_SYNCBUSY_Msk);
}

void WDT_Disable(void) {

}

void WDT_Acknowledge(void) {
    WDT_REGS->WDT_CLEAR = WDT_CLEAR_CLEAR_KEY_Val;

    while((WDT_REGS->WDT_STATUS & WDT_STATUS_SYNCBUSY_Msk) == WDT_STATUS_SYNCBUSY_Msk);
}

void WDT_SelfReset(void) {
    WDT_REGS->WDT_CLEAR = ~(WDT_CLEAR_CLEAR_KEY_Val);
}
