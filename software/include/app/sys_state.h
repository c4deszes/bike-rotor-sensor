#if !defined(APP_SYS_STATE_H_)
#define APP_SYS_STATE_H_

#define SYS_STATE_RESET_DELAY 100
#define SYS_STATE_BOOT_ENTRY_DELAY 100

void SYSSTATE_Initialize(void);

void SYSSTATE_Update(void);

#endif // APP_SYS_STATE_H_
