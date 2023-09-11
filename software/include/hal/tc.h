#if !defined(HAL_TC_H_)
#define HAL_TC_H_

#include "atsamd21e18a.h"

#define TC4 TC4_REGS

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

typedef struct {
    // mode
    // no wavegen
    // enable
    // presync??
    // prescaler
    tc_prescaler prescaler;
} tc_capture_config;

void TC_InitializeCapture(tc_registers_t* peripheral, tc_capture_config* config);

void TC_Start();

void TC_Stop();

#endif // HAL_TC_H_
