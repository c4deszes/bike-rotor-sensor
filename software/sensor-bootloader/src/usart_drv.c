#include "bootloader/usart_drv.h"

#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define RX_BUFFER_SIZE 64
uint8_t rx_buffer[RX_BUFFER_SIZE];
uint8_t rx_buffer_head;
uint8_t rx_buffer_tail;

void usart_push(uint8_t data) {
    rx_buffer[rx_buffer_head] = data;
    rx_buffer_head = (rx_buffer_head + 1) % RX_BUFFER_SIZE;
}

ISR(USART0_RXC_vect) {
    usart_push(USART0.RXDATAL);
}

uint8_t usart_available() {
    return (RX_BUFFER_SIZE + rx_buffer_head - rx_buffer_tail) % RX_BUFFER_SIZE;
}

uint8_t* usart_read() {
    if (rx_buffer_head == rx_buffer_tail) {
        return NULL;
    }
    uint8_t* data = &(rx_buffer[rx_buffer_tail]);
    rx_buffer_head = (rx_buffer_head + 1) % RX_BUFFER_SIZE;
    return data;
}