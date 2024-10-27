#include "app/spm.h"
#include "app/iet.h"
#include "bsp/sensor.h"
#include "app/config.h"

#include "app/sec.h"

// external config:
// - channel allocation
// - wheel sizes

/**  */
static const uint32_t spm_wheel_size_um = 2275000ul;

/** Global variables */
spm_channel_status_t spm_front_wheel;
spm_channel_status_t spm_rear_wheel;

spm_speed_t spm_global_speed;
spm_speed_state_t spm_global_state;

/** Local variables */

void SPM_Initialize(void) {
    spm_global_speed = 0;
    spm_global_state = spm_speed_state_ok;

    spm_front_wheel.state = spm_speed_state_ok;
    spm_front_wheel.speed = 0;

    spm_rear_wheel.state = spm_speed_state_ok;
    spm_rear_wheel.speed = 0;
}

// return 0.1 km/h / inc -> 70km/h = 700inc
// 1. wheelsize / period (m/s)
// 2. speed / 3.6 (km/h)
// 3. 
uint16_t SPM_PeriodToSpeed(uint32_t period_us) {
    // Period = microseconds
    // WheelSize = millimeters
    // Result = meters / second
    if (period_us > SPM_LOW_SPEED_CUTOFF_PERIOD_US) {
        return 0;
    }

    uint32_t speed = ((spm_wheel_size_um * 360U) / (period_us * 10U * 30U));

    if (speed > 700) {
        return 700;
    }
    return (uint16_t) speed;
}

static void SPM_UpdateChannel(uint8_t channel, spm_channel_status_t* status) {
    if (SENSOR_HasData(channel)) {
        osh_sensor_sample_t sample = SENSOR_GetSample(channel);
        
        status->speed = SPM_PeriodToSpeed(sample.period);
        status->state = spm_speed_state_ok;
    }

    sec_state_t channel_state = SEC_GetChannelState(channel);
    if (channel_state != sec_state_ok && channel_state != sec_state_warning) {
        status->state = spm_speed_state_error;
    }
}

static spm_speed_t SPM_InertialCompensation(spm_speed_t absoluteSpeed, iet_speed_delta_t inertial) {
    if (inertial > 0) {
        return absoluteSpeed + inertial;
    }
    else if (inertial < 0) {
        if(-inertial > absoluteSpeed) {
            return 0;
        }
        else {
            return absoluteSpeed + inertial;
        }
    }
    return absoluteSpeed;
}

static void SPM_UpdateGlobalState() {
    spm_speed_t global_sensor_speed = 0;
    spm_speed_state_t global_speed_state = spm_speed_state_error;
    iet_estimate_state_t iet_state = IET_GetEstimateState();
    iet_speed_delta_t iet_compensation = IET_GetEstimatedSpeed();
    
    if ((spm_front_wheel.state == spm_speed_state_ok || spm_rear_wheel.state == spm_speed_state_ok) && iet_state == iet_estimate_state_ok) {
        // at least one absolute + inertial compensation
        if (spm_front_wheel.state == spm_speed_state_ok && spm_rear_wheel.state == spm_speed_state_ok) {
            global_sensor_speed = SPM_InertialCompensation((spm_front_wheel.speed + spm_rear_wheel.speed) / 2, iet_compensation);
            global_speed_state = spm_speed_state_ok;
        }
        else if (spm_rear_wheel.state == spm_speed_state_ok) {
            global_sensor_speed = SPM_InertialCompensation(spm_rear_wheel.speed, iet_compensation);
            global_speed_state = spm_speed_state_ok;
        }
        else if (spm_front_wheel.state == spm_speed_state_ok) {
            global_sensor_speed = SPM_InertialCompensation(spm_front_wheel.speed, iet_compensation);
            global_speed_state = spm_speed_state_ok;
        }
    }
    else if (spm_front_wheel.state == spm_speed_state_ok || spm_rear_wheel.state == spm_speed_state_ok) {
        // at least one absolute
        if (spm_front_wheel.state == spm_speed_state_ok && spm_rear_wheel.state == spm_speed_state_ok) {
            global_sensor_speed = (spm_front_wheel.speed + spm_rear_wheel.speed) / 2;
            global_speed_state = spm_speed_slow_response;
        }
        else if (spm_rear_wheel.state == spm_speed_state_ok) {
            global_sensor_speed = spm_rear_wheel.speed;
            global_speed_state = spm_speed_slow_response;
        }
        else if (spm_front_wheel.state == spm_speed_state_ok) {
            global_sensor_speed = spm_front_wheel.speed;
            global_speed_state = spm_speed_slow_response;
        }
    }
    else if (iet_state == iet_estimate_state_ok) {
        // inertial only, use last ok global speed
        // TODO: use latest valid absolute speed + estimate
        global_speed_state = spm_speed_state_unreliable;
        global_sensor_speed = 0;
    }
    else {
        // multiple failures
        // TODO: if either channels report unreliable then return with that
        //       if both channels report error then return with that
        global_speed_state = spm_speed_state_error;
        global_sensor_speed = 0;
    }

    spm_global_speed = global_sensor_speed;
    spm_global_state = global_speed_state;
}

void SPM_Update(void) {
    SPM_UpdateChannel(SPM_FRONT_CHANNEL, &spm_front_wheel);
    SPM_UpdateChannel(SPM_REAR_CHANNEL, &spm_rear_wheel);

    SPM_UpdateGlobalState();
}
