/**
 * @file itpms.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-12-05
 * 
 * @copyright Copyright (c) 2023
 * 
 * The iTPMS service is responsible for monitoring each wheel speed and
 * comparing them over time
 * 
 */
#if !defined(APP_ITPMS_)
#define APP_ITPMS_

typedef enum {
    itpms_state_stopped,
    itpms_state_running,
    itpms_state_front_pressure_low,
    itpms_state_rear_pressure_low,
} itpms_state_t;

extern itpms_state_t itpms_state;

/**
 * @brief Initializes the tire pressure monitoring service
 */
void ITPMS_Initialize(void);

/**
 * @brief Updates the sensed tire pressure values
 */
void ITPMS_Update(void);

void ITPMS_Start(void);

void ITPMS_Stop(void);

#endif // APP_ITPMS_
