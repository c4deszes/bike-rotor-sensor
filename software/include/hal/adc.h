#if !defined(HAL_ADC_H_)
#define HAL_ADC_H_

#include <stdint.h>

// Loads calib values from NVRAM
void ADC_LoadCalibration(void);

void ADC_SetupFreerun(uint8_t channel);

uint16_t ADC_ImmediateRead(uint8_t channel);

#endif // HAL_ADC_H_
