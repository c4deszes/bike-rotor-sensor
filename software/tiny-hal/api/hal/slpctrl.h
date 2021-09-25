#pragma once

typedef enum {
    SLPCTRL_SLEEP_MODE_IDLE,
    SLPCTRL_SLEEP_MODE_STANDBY,
    SLPCTRL_SLEEP_MODE_PDOWN
} slpctrl_sleep_mode;

/**
 * @brief 
 * 
 */
void slpctrl_sleep(slpctrl_sleep_mode mode);
