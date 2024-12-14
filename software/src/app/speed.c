#include "app/speed.h"
#include "app/imu.h"
#include "app/config.h"
#include "uds_gen.h"

#include "app/sec.h"

/** Local variables */
uint16_t SPEED_GlobalSpeed = 0;
speed_status_t SPEED_GlobalStatus = speed_status_ok;

speed_channel_status_t SPEED_FrontWheel = {
    .state = speed_status_ok,
    .speed = 0,
    .brake = false,
    .lockup = false,
    .slip = false
};

speed_channel_status_t SPEED_RearWheel = {
    .state = speed_status_ok,
    .speed = 0,
    .brake = false,
    .lockup = false,
    .slip = false
};

void SPEED_Initialize(void) {

}

void SPEED_Update(void) {
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

    // TODO: inertial speed compensation, also based on sensor timestamps

    if (SPEED_FrontWheel.state == speed_status_ok && SPEED_RearWheel.state == speed_status_ok) {
        SPEED_GlobalSpeed = (SPEED_FrontWheel.speed + SPEED_RearWheel.speed) / 2;
        SPEED_GlobalStatus = speed_status_ok;
    }
    else if (SPEED_FrontWheel.state == speed_status_ok) {
        SPEED_GlobalSpeed = SPEED_FrontWheel.speed;
        SPEED_GlobalStatus = speed_status_ok;
    }
    else if (SPEED_RearWheel.state == speed_status_ok) {
        SPEED_GlobalSpeed = SPEED_RearWheel.speed;
        SPEED_GlobalStatus = speed_status_ok;
    }
    else {
        SPEED_GlobalSpeed = 0;
        SPEED_GlobalStatus = speed_status_error;
    }
}

static uint16_t SPEED_CalculateSpeed(uint32_t period, uint8_t poles, uint16_t circumference) {
    // TODO: for 0.1 km/h resolution, we need to measure the period in 0.1ms resolution
    // TODO: prevent 0 division
    // TODO: limit output range
    if (period == 0 || poles == 0 || circumference == 0) {
        return 0;
    }
    return (uint16_t)((36000UL * circumference) / (period * poles));
}

void SPEED_OnTick(uint8_t channel, osh_sensor_sample_t sample) {
    if (channel == SPM_FRONT_SENSOR_CHANNEL) {
        SPEED_FrontWheel.last_period = sample.period;
        SPEED_FrontWheel.speed_cnt = sample.period;
        //SPEED_FrontWheel.state = sample.state;
        // TODO: for 0.1 km/h resolution, we need to measure the period in 0.1ms resolution
        SPEED_FrontWheel.speed = SPEED_CalculateSpeed(SPEED_FrontWheel.last_period,
                                                     UDS_AppContainer.FrontWheel_PoleCount,
                                                     UDS_AppContainer.FrontWheel_Circumference);
    }
    else if (channel == SPM_REAR_SENSOR_CHANNEL) {
        SPEED_RearWheel.last_period = sample.period;
        SPEED_RearWheel.speed_cnt = sample.period;
        //SPEED_RearWheel.state = sample.state;
        // TODO: for 0.1 km/h resolution, we need to measure the period in 0.1ms resolution
        SPEED_RearWheel.speed = SPEED_CalculateSpeed(SPEED_RearWheel.last_period,
                                                    UDS_AppContainer.RearWheel_PoleCount,
                                                    UDS_AppContainer.RearWheel_Circumference);
    }
}

uint16_t SPEED_GetSpeed() {
    return SPEED_GlobalSpeed;
}

speed_status_t SPEED_GetStatus() {
    return SPEED_GlobalStatus;
}
