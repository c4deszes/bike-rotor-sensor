#if !defined(HAL_RTC_H_)
#define HAL_RTC_H_

#include <stdint.h>

void RTC_Initialize(void);

void RTC_Start(void);

uint32_t RTC_GetCount(void);

#endif // HAL_RTC_H_
