#if !defined(__APP_VOLT_H__)
#define __APP_VOLT_H__

#include <stdint.h>

void VOLT_Initialize(void);

void VOLT_Update(void);

uint16_t VOLT_GetVoltage(void);

#endif // __APP_VOLT_H__
