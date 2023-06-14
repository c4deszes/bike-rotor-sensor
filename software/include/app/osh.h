#if !defined(APP_OUTPUT_STAGE_H_)
#define APP_OUTPUT_STAGE_H_

#include <stdbool.h>
#include <stdint.h>

void osh_init(void);

void osh_update(void);

// Turn on both channels (operational)
void osh_turn_on(void);

// Turn off both channels (low current)
void osh_turn_off(void);

#endif // APP_OUTPUT_STAGE_H_
