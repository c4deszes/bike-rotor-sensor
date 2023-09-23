#if !defined(APP_OUTPUT_STAGE_H_)
#define APP_OUTPUT_STAGE_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    OSH_STATE_OFF,
    OSH_STATE_OK,
    //OSH_STATE_REVERSE,
    OSH_STATE_SENSOR_WARNING,
    //OSH_STATE_SPEED_UNRELIABLE,
    OSH_STATE_SENSOR_ERROR,
    OSH_STATE_PHY_SHORT,
    OSH_STATE_PHY_OPEN
} osh_state;

/**
 * @brief Initializes the Output stage handler
 * 
 * This includes initializing the physical layer
 */
void OSH_Initialize(void);

/**
 * @brief Updates the output stage 
 * 
 */
void OSH_Update(void);

osh_state OSH_GetState(uint8_t channel);

/**
 * @brief Turns on all channels of the output stage
 * 
 * The actual operation might be deferred until the next update cycle.
 */
void OSH_TurnOn(void);

/**
 * @brief Turns off all channels of the output stage
 * 
 * The actual operation might be deferred until the next update cycle.
 */
void OSH_TurnOff(void);

#endif // APP_OUTPUT_STAGE_H_
