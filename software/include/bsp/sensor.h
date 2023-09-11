#if !defined(BSP_OSH_PROC_H_)
#define BSP_OSH_PROC_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    osh_sensor_direction_unknown,
    osh_sensor_direction_forward,
    osh_sensor_direction_reverse,
    osh_sensor_direction_error
} osh_sensor_direction;

typedef enum {
    osh_sensor_state_ok,
    osh_sensor_state_warning,
    osh_sensor_state_error
} osh_sensor_state;

/**
 * @brief Funtion is called by the timer interrupt
 * 
 * @param channel Channel number
 * @param width pulse width in microseconds
 * @param period pulse period in microseconds
 */
void osh_sensor_process(uint8_t channel, uint32_t width, uint32_t period);

/**
 * @brief Returns the direction of rotation
 * 
 * @param channel 
 * @return osh_sensor_direction 
 */
osh_sensor_direction osh_sensor_get_direction(uint8_t channel);

/**
 * @brief Returns the sensor state
 * 
 * @param channel Channel number
 * @return osh_sensor_state sensor state (ok, error, warning)
 */
osh_sensor_state osh_sensor_get_state(uint8_t channel);

/**
 * @brief Returns speed information in milli-hertz
 * 
 * @param channel 
 * @return uint16_t 
 */
uint16_t osh_sensor_get_data(uint8_t channel);

/**
 * @brief Returns the amount of data available 
 * 
 * @param channel Channel number
 * @return uint8_t number of speed captures
 */
uint8_t osh_sensor_has_data(uint8_t channel);

/**
 * @brief Puts speed data in the given channel's buffer
 * 
 * @param channel Channel number
 * @param speed 
 * @return true if data was put successfully in the buffer
 * @return false if the buffer is full
 */
bool osh_sensor_put_data(uint8_t channel, uint16_t speed);

#endif // BSP_OSH_PROC_H_
