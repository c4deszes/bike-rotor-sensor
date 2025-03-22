#if !defined(APP_SENSOR_CONTROL_H_)
#define APP_SENSOR_CONTROL_H_

/**
 * @file sensor_control.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-03
 * 
 * @copyright Copyright (c) 2023
 * 
 * Sensor control is responsible for the output stages and the connected sensors:
 * - during startup it enables the outputs
 * - in case of failures the output stage or sensor might be reset
 */

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    // DO NOT CHANGE ORDER
    sec_state_ok,
    sec_state_warning,
    sec_state_error,
    sec_state_open,
    sec_state_short,
    sec_state_off
} sec_state_t;

/**
 * @brief Initializes the sensors
 */
void SEC_Initialize(void);

/**
 * @brief Updates the output stage 
 * 
 */
void SEC_Update(void);

/**
 * @brief Returns the channel state
 * 
 * @param channel 
 * @return sec_state_t 
 */
sec_state_t SEC_GetChannelState(uint8_t channel);

/**
 * @brief Turns on all speed sensors
 */
void SEC_TurnOn(void);

/**
 * @brief Turns off all speed sensors
 */
void SEC_TurnOff(void);

#endif // APP_SENSOR_CONTROL_H_
