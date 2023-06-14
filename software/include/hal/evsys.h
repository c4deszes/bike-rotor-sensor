#if !defined(HAL_EVSYS_)
#define HAL_EVSYS_

#include <stdint.h>

typedef enum {
    NO_EVT_OUTPUT = 0x0,
    RISING_EDGE = 0x1,
    FALLING_EDGE = 0x2,
    BOTH_EDGES = 0x03
} evsys_gen_edgesel;

typedef enum {
    SYNCHRONOUS = 0x0,
    RESYNCHRONIZED = 0x01,
    ASYNCHRONOUS = 0x2
} evsys_gen_path;

void EVSYS_ConfigureGenerator(uint8_t channel, evsys_gen_edgesel edgsel, evsys_gen_path path, uint8_t event_generator);

void EVSYS_ConfigureUser(uint8_t user_channel, uint8_t event_generator);

#endif // HAL_EVSYS_
