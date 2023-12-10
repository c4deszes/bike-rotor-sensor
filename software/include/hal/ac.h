#if !defined(HAL_AC_H_)
#define HAL_AC_H_

#include <stdint.h>
#include <stdbool.h>

//void AC_SetupComparator(uint8_t comp, );

bool AC_GetComparatorState(uint8_t comp);

void AC_Enable(void);

void AC_Reset(void);

#endif // HAL_AC_H_
