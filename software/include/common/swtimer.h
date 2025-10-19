#if !defined(COMMON_SW_TIMER_H_)
#define COMMON_SW_TIMER_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint16_t millis;
} swtimer_t;

swtimer_t* SWTIMER_Create(void);

void SWTIMER_Update1ms();

void SWTIMER_Setup(swtimer_t* timer, uint16_t millis);

bool SWTIMER_Elapsed(swtimer_t* timer);

#endif // COMMON_SW_TIMER_H_
