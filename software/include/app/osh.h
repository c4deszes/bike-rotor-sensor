#if !defined(APP_OUTPUT_STAGE_H_)
#define APP_OUTPUT_STAGE_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    OSH_STATE_OFF,
    OSH_STATE_OK,
    //OSH_STATE_REVERSE,
    OSH_STATE_SENSOR_WARNING,
    OSH_STATE_SPEED_UNRELIABLE,
    OSH_STATE_SENSOR_ERROR,
    OSH_STATE_PHY_SHORT,
    OSH_STATE_PHY_OPEN
} osh_state;

void osh_init(void);

void osh_update(void);

osh_state osh_get_state(uint8_t channel);

// Turn on both channels (operational)
void osh_turn_on(void);

// Turn off both channels (low current)
void osh_turn_off(void);

#endif // APP_OUTPUT_STAGE_H_
