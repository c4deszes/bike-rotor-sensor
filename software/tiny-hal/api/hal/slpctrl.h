#pragma once

typedef enum {
    SLPCTRL_SLEEP_MODE_IDLE,        /**< Code execution is halted but any interrupt will wakeup */
    SLPCTRL_SLEEP_MODE_STANDBY,     /**< CPU halts, only interrupts from peripherals with RUNSTDBY will cause wakeup*/
    SLPCTRL_SLEEP_MODE_PDOWN        /**< Only a fixed selection of peripherals can cause wakeup*/
} slpctrl_sleep_mode;

/**
 * @brief Sleeps the microcontroller
 */
void slpctrl_sleep(slpctrl_sleep_mode mode);
