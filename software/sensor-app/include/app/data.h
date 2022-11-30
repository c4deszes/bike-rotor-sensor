#include "hal/nvmctrl.h"
#include <stdint.h>

typedef struct {
    uint8_t pulse_per_rotation;
    uint8_t index_threshold;
} config_layout_t;

extern config_layout_t app_config;
