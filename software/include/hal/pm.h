#ifndef RSTC_H_
#define RSTC_H_

/**
 * @brief 
 * 
 */
typedef enum {
    PM_RCAUSE_INVALID,
    PM_RCAUSE_SYSTEM_RESET,                        /**< Cortex System reset */
    PM_RCAUSE_WATCHDOG,                            /**< Watchdog reset */
    PM_RCAUSE_EXTERNAL,                            /**< External reset */
    PM_RCAUSE_NVM,
    PM_RCAUSE_BOD33,
    PM_RCAUSE_BOD12,
    PM_RCAUSE_POR
} pm_rcause;

/**
 * @brief Returns the reset cause
 * 
 * @return PM_RCAUSE Reset cause
 */
extern pm_rcause PM_ResetCause(void);

void PM_IdleModeEnter(void);

void PM_StandbyModeEnter(void);

#endif
