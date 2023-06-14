#if !defined(BSP_OSH_PROC_H_)
#define BSP_OSH_PROC_H_

#include <stdint.h>

typedef enum {
    osh_sensor_direction_forward,
    osh_sensor_direction_reverse
} osh_sensor_direction;

void osh_sensor_process(uint16_t width, uint16_t period);

uint16_t osh_sensor_get_speed(void);

osh_sensor_direction osh_sensor_get_direction(void);

// TODO: TLE5046 has airgap warning

#endif // BSP_OSH_PROC_H_
