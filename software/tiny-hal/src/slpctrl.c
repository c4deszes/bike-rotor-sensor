#include "hal/slpctrl.h"

#include <avr/io.h>

inline uint8_t slpctrl_get_mode_reg(const slpctrl_sleep_mode mode) {
    switch (mode) {
        case SLPCTRL_SLEEP_MODE_IDLE: return SLPCTRL_SMODE_IDLE_gc;
        case SLPCTRL_SLEEP_MODE_STANDBY: return SLPCTRL_SMODE_STDBY_gc;
        case SLPCTRL_SLEEP_MODE_PDOWN: return SLPCTRL_SMODE_PDOWN_gc;
        default: return 0;
    }
    return 0;
}

void slpctrl_sleep(const slpctrl_sleep_mode mode) {
    uint8_t smode = (slpctrl_get_mode_reg(mode) << SLPCTRL_SMODE_gp) & SLPCTRL_SMODE_gm;
    SLPCTRL.CTRLA = smode | SLPCTRL_SEN_bm;

    __asm__( "sleep;" );
}
