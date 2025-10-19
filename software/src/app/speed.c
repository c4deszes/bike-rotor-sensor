#include "app/speed.h"
#include "app/imu.h"
#include "app/config.h"

#include "app/sec.h"

/** Local variables */
static uint16_t SPEED_GlobalSpeed = 0;
static speed_status_t SPEED_GlobalStatus = speed_status_ok;

speed_channel_status_t SPEED_FrontWheel;
speed_channel_status_t SPEED_RearWheel;

void SPEED_Initialize(void) {
    SPEED_GlobalSpeed = 0;
    SPEED_GlobalStatus = speed_status_ok;

    SPEED_FrontWheel.state = speed_status_ok;
    SPEED_FrontWheel.speed = 0;
    SPEED_FrontWheel.last_period = UINT16_MAX;
    SPEED_FrontWheel.period_cnt = 0;
    SPEED_FrontWheel.brake = false;
    SPEED_FrontWheel.lockup = false;
    SPEED_FrontWheel.slip = false;

    SPEED_RearWheel.state = speed_status_ok;
    SPEED_RearWheel.speed = 0;
    SPEED_RearWheel.last_period = UINT16_MAX;
    SPEED_RearWheel.period_cnt = 0;
    SPEED_RearWheel.brake = false;
    SPEED_RearWheel.lockup = false;
    SPEED_RearWheel.slip = false;
}

/**
 * @brief Calculates immediate speed from the pulse period and wheel parameters
 * 
 * If the parameters are invalid, the function returns 0.
 * If the period is too long, the function clamps the speed to 0.
 * If the period is too short, the function clamps the speed to the maximum speed.
 * 
 * @param period Period in 100us increments
 * @param poles Number of poles on the wheel
 * @param circumference Wheel circumference in mm
 * @return uint16_t Speed in 0.1 km/h increments
 */
static inline uint16_t SPEED_CalculateSpeed(uint32_t period, uint8_t poles, uint16_t circumference) {
    if (period == 0 || poles == 0 || circumference == 0) {
        return 0;
    }

    if (period > SPM_LOW_SPEED_CUTOFF_PERIOD_100US) {
        return 0;
    }
    else if (period < SPM_HIGH_SPEED_CUTOFF_PERIOD_US) {
        return (360UL * circumference) / (SPM_HIGH_SPEED_CUTOFF_PERIOD_US * poles);
    }
    else {
        return (uint16_t)((360UL * circumference) / (period * poles));
    }
}

/*
 * Two sensor speed calculation algorithms:
 * 1. LastDerate
 *    takes the last speed of the two sensors and derates it once the last data point is older than a certain threshold
 */

static inline uint16_t SPEED_Calculate_Derate(speed_channel_status_t* channel) {
    // TODO: differentiate between front and rear wheel
    return SPEED_CalculateSpeed(channel->last_period > channel->period_cnt ? channel->last_period : channel->period_cnt,
                                            CONFIG_Props.FrontWheel_PoleCount,
                                            CONFIG_Props.FrontWheel_Circumference);
}

static inline uint16_t SPEED_CalculateTwoSensors_DerateLast(speed_channel_status_t* front, speed_channel_status_t* rear) {
    if (front->period_cnt < rear->period_cnt) {
        return SPEED_Calculate_Derate(front);
    }
    else {
        return SPEED_Calculate_Derate(rear);
    }
}

void SPEED_Update(void) {
    /**
     * Global speed calculation
     * 
     * If both sensors are ok, the global speed should be based on the average of the two sensors
     * accounting for the fact when the sensor data was acquired.
     * 
     * If only one sensor is ok, the global speed should be based on the speed of the working sensor.
     * 
     * If both sensors are not ok, the global speed should be 0.
     */
    if (SPEED_FrontWheel.period_cnt < SPM_LOW_SPEED_CUTOFF_PERIOD_100US) {
        SPEED_FrontWheel.period_cnt += 100;
    }
    if (SPEED_RearWheel.period_cnt < SPM_LOW_SPEED_CUTOFF_PERIOD_100US) {
        SPEED_RearWheel.period_cnt += 100;
    }

    if (SEC_GetChannelState(SPM_FRONT_SENSOR_CHANNEL) != sec_state_ok && SEC_GetChannelState(SPM_FRONT_SENSOR_CHANNEL) != sec_state_off) {
        SPEED_FrontWheel.state = speed_status_error;
    }
    else {
        SPEED_FrontWheel.state = speed_status_ok;
    }

    if (SEC_GetChannelState(SPM_REAR_SENSOR_CHANNEL) != sec_state_ok && SEC_GetChannelState(SPM_REAR_SENSOR_CHANNEL) != sec_state_off) {
        SPEED_RearWheel.state = speed_status_error;
    }
    else {
        SPEED_RearWheel.state = speed_status_ok;
    }

    // TODO: inertial speed compensation, also based on sensor timestamps

    if (SPEED_FrontWheel.state == speed_status_ok && SPEED_RearWheel.state == speed_status_ok) {
        SPEED_GlobalSpeed = SPEED_CalculateTwoSensors_DerateLast(&SPEED_FrontWheel, &SPEED_RearWheel);
        SPEED_GlobalStatus = speed_status_ok;
    }
    else if (SPEED_FrontWheel.state == speed_status_ok) {
        SPEED_GlobalSpeed = SPEED_Calculate_Derate(&SPEED_FrontWheel);
        SPEED_GlobalStatus = speed_status_ok;
    }
    else if (SPEED_RearWheel.state == speed_status_ok) {
        SPEED_GlobalSpeed = SPEED_Calculate_Derate(&SPEED_RearWheel);
        SPEED_GlobalStatus = speed_status_ok;
    }
    else {
        SPEED_GlobalSpeed = 0;
        SPEED_GlobalStatus = speed_status_error;
    }
}

void SPEED_OnTick(uint8_t channel, osh_sensor_sample_t sample) {
    if (channel == SPM_FRONT_SENSOR_CHANNEL) {
        SPEED_FrontWheel.last_period = sample.period;
        SPEED_FrontWheel.period_cnt = 0;
        SPEED_FrontWheel.speed = SPEED_CalculateSpeed(SPEED_FrontWheel.last_period,
                                                        CONFIG_Props.FrontWheel_PoleCount,
                                                        CONFIG_Props.FrontWheel_Circumference);

        // TODO: if the period is out of range then increase error counter
        // TODO: if the acceleration is not plausible then increase error counter
    }
    else if (channel == SPM_REAR_SENSOR_CHANNEL) {
        SPEED_RearWheel.last_period = sample.period;
        SPEED_RearWheel.period_cnt = 0;
        SPEED_RearWheel.speed = SPEED_CalculateSpeed(SPEED_RearWheel.last_period,
                                                        CONFIG_Props.RearWheel_PoleCount,
                                                        CONFIG_Props.RearWheel_Circumference);
    }
}

uint16_t SPEED_GetSpeed() {
    return SPEED_GlobalSpeed;
}

speed_status_t SPEED_GetStatus() {
    return SPEED_GlobalStatus;
}
