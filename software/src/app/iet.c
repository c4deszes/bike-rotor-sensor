#include "app/iet.h"

static iet_speed_delta_t speed;
static iet_estimate_state_t state;

void IET_Initialize(void) {
    speed = 0;
    state = iet_estimate_state_error;
}

void IET_Update(void) {
    /*
    if (BMI088_GetState() == bmi088_state_error) {
        state = iet_estimate_state_error;
    }
    else {
        bool new_data = BMI088_HasNewData();
        if (new_data && isValidAcceleration(BMI088_GetAcceleration())) {
            if (state == iet_estimate_state_error) {
                state = iet_estimate_state_ok;
                speed = 0;
            }
            speed += BMI088_GetAcceleration()
        }

        if (new_data && !isValidAcceleration(BMI088_GetAcceleration())) {
            state = iet_estimate_state_error;
        }
    }
    
    */
}

void IET_ZeroOut(void) {
    speed = 0;
}

iet_speed_delta_t IET_GetEstimatedSpeed(void) {
    return speed;
}

iet_estimate_state_t IET_GetEstimateState(void) {
    return state;
}
