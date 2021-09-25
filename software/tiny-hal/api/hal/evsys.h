#pragma once

#include <stdint.h>

// Sync
void evsys_gen_sync_trigger(const uint8_t channel);

void evsys_gen_sync_select(const uint8_t channel, const uint8_t source);

void evsys_gen_sync_disable(const uint8_t channel);

// Async gen
void evsys_gen_async_trigger(const uint8_t channel);

void evsys_gen_async_select(const uint8_t channel, const uint8_t source);

void evsys_gen_async_disable(const uint8_t channel);

// Async user
void evsys_user_async_select(const uint8_t channel, const uint8_t event);

void evsys_user_sync_select(const uint8_t channel, const uint8_t event);
