#include "hal/evsys.h"

#include <avr/io.h>

void evsys_gen_sync_trigger(const uint8_t channel) {
    EVSYS.SYNCSTROBE = _BV(channel);
}

void evsys_gen_sync_select(const uint8_t channel, const uint8_t source) {
    volatile uint8_t* syncch = &(EVSYS.SYNCCH0);
    syncch[channel] = source;
}

void evsys_gen_sync_disable(const uint8_t channel) {
    evsys_gen_sync_select(channel, 0);
}

void evsys_gen_async_trigger(const uint8_t channel) {
    EVSYS.ASYNCSTROBE = _BV(channel);
}

void evsys_gen_async_select(const uint8_t channel, const uint8_t source) {
    volatile uint8_t* asyncch = &(EVSYS.ASYNCCH0);
    asyncch[channel] = source;
}

void evsys_gen_async_disable(const uint8_t channel) {
    evsys_gen_async_select(channel, 0);
}

void evsys_user_async_select(const uint8_t channel, const uint8_t generator) {
    volatile uint8_t* userch = &(EVSYS.ASYNCUSER0);
    userch[channel] = generator;
}

void evsys_user_sync_select(const uint8_t channel, const uint8_t generator) {
    volatile uint8_t* userch = &(EVSYS.SYNCUSER0);
    userch[channel] = generator;
}
