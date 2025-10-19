#if !defined(HAL_TCC_H_)
#define HAL_TCC_H_

#include <stdint.h>

#define TCC0 0
#define TCC1 1

// TODO: support for other things, like prescaler, mode, interrupts
void TCC_SetupTrigger(uint8_t timer, uint32_t period);

void TCC_Enable(uint8_t timer);

// TODO: TCC_Disable();

void TCC_Reset(uint8_t timer);

// TODO: update signature to include int. cause
void TCC0_Interrupt(void);

// TODO: update signature to include int. cause
void TCC1_Interrupt(void);

#endif // HAL_TCC_H_
