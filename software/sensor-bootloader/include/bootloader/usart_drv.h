#pragma once

#include <stdint.h>

void usart_push(uint8_t data);

uint8_t usart_available();

uint8_t* usart_read();
