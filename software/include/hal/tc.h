#if !defined(HAL_TC_H_)
#define HAL_TC_H_

#include <stdint.h>

#define TC3 3
#define TC4 4
#define TC5 5

typedef enum {
    tc_prescaler_div1 = 0x0,
    tc_prescaler_div2 = 0x1,
    tc_prescaler_div4 = 0x2,
    tc_prescaler_div8 = 0x3,
    tc_prescaler_div16 = 0x4,
    tc_prescaler_div64 = 0x5,
    tc_prescaler_div256 = 0x6,
    tc_prescaler_div1024 = 0x7
} tc_prescaler;

void TC_SetupCapture(uint8_t timer, tc_prescaler prescaler);

void TC_Start(uint8_t timer);

void TC_Stop(uint8_t timer);

#endif // HAL_TC_H_
