#if !defined(__APP_CURRENT_H__)
#define __APP_CURRENT_H__

#include <stdint.h>

void CURRENT_Initialize(void);

void CURRENT_Update(void);

uint16_t CURRENT_GetCurrent(void);

#endif // __APP_CURRENT_H__
