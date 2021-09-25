#pragma once

typedef enum {
    RSTCTRCL_CAUSE_UPDI,
    RSTCTRCL_CAUSE_SOFTWARE,
    RSTCTRCL_CAUSE_WATCHDOG,
    RSTCTRCL_CAUSE_EXTERNAL,
    RSTCTRCL_CAUSE_BROWN_OUT,
    RSTCTRCL_CAUSE_POWER_ON
} rstctrl_cause;

/**
 * @brief Returns the reset cause
 * 
 * @return rstctrl_cause 
 */
rstctrl_cause rstctrl_get_cause();

/**
 * @brief 
 * 
 */
void rstctrl_software_reset(void);
