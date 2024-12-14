#if !defined(APP_SPM_H_)
#define APP_SPM_H_

#include <stdint.h>
#include <stdbool.h>

#include "bsp/sensor.h"

typedef enum {
    speed_status_ok,             // Set when the speed if fine
    speed_status_slow_response,        // Set when the IMU failed
    speed_status_unreliable,     // Set when the speed sensors failed (IMU alt. mode) or speed plausability fail
    speed_status_error           // Set when both speed sensors and IMU failed
} speed_status_t;

typedef struct {
    speed_status_t state;
    uint16_t speed;
    uint32_t last_period;
    uint32_t speed_cnt;
    bool brake;
    bool lockup;
    bool slip;
} speed_channel_status_t;

extern speed_channel_status_t SPEED_FrontWheel;
extern speed_channel_status_t SPEED_RearWheel;

void SPEED_Initialize(void);

void SPEED_Update(void);

void SPEED_OnTick(uint8_t channel, osh_sensor_sample_t sample);

uint16_t SPEED_GetSpeed();

speed_status_t SPEED_GetStatus();

#endif // APP_SPM_H_
