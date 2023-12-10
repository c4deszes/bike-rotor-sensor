#if !defined(APP_SPM_H_)
#define APP_SPM_H_

#include <stdint.h>
#include <stdbool.h>

typedef uint16_t spm_speed_t;

typedef enum {
    spm_speed_state_ok,             // Set when the speed if fine
    spm_speed_slow_response,        // Set when the IMU failed
    spm_speed_state_unreliable,     // Set when the speed sensors failed (IMU alt. mode) or speed plausability fail
    spm_speed_state_error           // Set when both speed sensors and IMU failed
} spm_speed_state_t;

typedef struct {
    //spm_speed_state_t state;
    spm_speed_t speed;
    bool brake;
    bool lockup;
    bool slip;
} spm_channel_status_t;

extern spm_channel_status_t spm_channels[2];

extern spm_speed_t spm_global_speed;
extern spm_speed_state_t spm_global_state;

/**
 * @brief Initializes the speed monitoring
 */
void SPM_Initialize(void);

void SPM_Update(void);

#endif // APP_SPM_H_
