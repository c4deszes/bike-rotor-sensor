#if !defined(BSP_OSH_PROC_H_)
#define BSP_OSH_PROC_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    osh_sensor_direction_unknown,
    osh_sensor_direction_forward,
    osh_sensor_direction_reverse,
    osh_sensor_direction_error
} osh_sensor_direction_t;

typedef enum {
    osh_sensor_state_ok,        // No issues with the sensor
    osh_sensor_state_warning,   // Sensor experiencing minor problems (e.g.: airgap, brown out)
    osh_sensor_state_error      // Sensor experiencing major problems, data is unreliable
} osh_sensor_state_t;

typedef struct {
    uint32_t pos;   // TODO: is 16 bit enough?
    uint32_t period;
} osh_sensor_sample_t;

void SENSOR_Initialize(void);

/**
 * @brief Funtion is called by the timer interrupt to process a capture pulse, the function should
 *        return as soon as possible
 * 
 * @param channel Channel number
 * @param width pulse width in microseconds
 * @param period pulse period in microseconds
 */
void SENSOR_Process(uint8_t channel, uint32_t width, uint32_t period);

osh_sensor_state_t SENSOR_GetState(uint8_t channel);

/**
 * @brief Returns speed information in milli-hertz
 * 
 * @param channel Channel number
 * @return osh_sensor_sample_t 
 */
osh_sensor_sample_t SENSOR_GetSample(uint8_t channel);

/**
 * @brief Returns the amount of samples available 
 * 
 * @param channel Channel number
 * @return uint8_t number of speed captures
 */
uint8_t SENSOR_HasData(uint8_t channel);

#endif // BSP_OSH_PROC_H_
