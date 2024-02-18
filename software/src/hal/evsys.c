#include "hal/evsys.h"
#include "atsamd21e18a.h"

static uint8_t generators = 0;

void EVSYS_ConfigureGenerator(uint8_t channel, evsys_gen_edgesel edgsel, evsys_gen_path path, uint8_t event_generator) {
    EVSYS_REGS->EVSYS_CHANNEL = (edgsel << EVSYS_CHANNEL_EDGSEL_Pos) |
                                (path << EVSYS_CHANNEL_PATH_Pos) |
                                (event_generator << EVSYS_CHANNEL_EVGEN_Pos) |
                                (channel);
}

void EVSYS_ConfigureUser(uint8_t user_channel, uint8_t event_generator) {
    EVSYS_REGS->EVSYS_USER = (event_generator << EVSYS_USER_CHANNEL_Pos) | user_channel;
}

uint8_t EVSYS_AcquireGeneratorChannel(void) {
    if (generators >= 16) {
        return 0xFF;
    }
    generators++;
    return generators;
}
