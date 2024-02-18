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

/**
 * @brief Configures the event generator
 * 
 * @code
 * EVSYS_ConfigureGenerator(0, RISING_EDGE, SYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_4)
 * @endcode
 * 
 * @param channel Generator channel number
 * @param edgsel Event generation edge mode
 * @param path Event generation mode
 * @param event_generator Event source
 */
void EVSYS_ConfigureGenerator(uint8_t channel, evsys_gen_edgesel edgsel, evsys_gen_path path, uint8_t event_generator);

/**
 * @brief Configures the event user
 * 
 * @code
 * EVSYS_ConfigureUser(EVENT_ID_USER_TC4_EVU, 0);
 * @endcode
 * 
 * @param user_channel Event's consumer
 * @param event_generator Generator channel
 */
void EVSYS_ConfigureUser(uint8_t user_channel, uint8_t event_generator);

/**
 * @brief Acquires a free generator channel
 * 
 * @code
 * // Connecting EXTINT4 event to TC4 capture event
 * uint8_t gen_channel = EVSYS_AcquireGeneratorChannel();
 * EVSYS_ConfigureGenerator(gen_channel, NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_4)
 * EVSYS_ConfigureUser(EVENT_ID_USER_TC4_EVU, gen_channel)
 * @endcode
 * 
 * @return uint8_t Generator channel number
 */
uint8_t EVSYS_AcquireGeneratorChannel(void);

#endif // HAL_EVSYS_
