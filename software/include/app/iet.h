/**
 * @file iet.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-04
 * 
 * @copyright Copyright (c) 2023
 * 
 * The Inertial estimator is responsible for:
 * - Estimating speed based on acceleration data
 */
#if !defined(APP_IET_H_)
#define APP_IET_H_

#include <stdint.h>
#include "app/spm.h"

typedef enum {
    iet_estimate_state_ok,
    iet_estimate_state_error
} iet_estimate_state_t;

typedef int8_t iet_speed_delta_t;

/**
 * @brief Initializes the Inertial estimator
 * - Set speed to zero
 */
void IET_Initialize(void);

/**
 * @brief Updates the estimated speed and inclination by the data received from the IMU
 */
void IET_Update(void);

/**
 * @brief Sets the current reference speed to zero
 */
void IET_ZeroOut(void);

iet_speed_delta_t IET_GetEstimatedSpeed(void);

iet_estimate_state_t IET_GetEstimateState(void);

#endif // APP_IET_H_
