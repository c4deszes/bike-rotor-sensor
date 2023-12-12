#include "hal/sercom_usart.h"
#include "bsp/pinout.h"

#include "bsp/usart.h"

void USART_Initialize(uint32_t baudrate, ringbuffer8_t* tx_buffer, ringbuffer8_t* rx_buffer) {
    GPIO_EnableFunction(UART_TX_PORT, UART_TX_PIN, UART_TX_PINMUX);
    GPIO_EnableFunction(UART_RX_PORT, UART_RX_PIN, UART_RX_PINMUX);
    SERCOM_USART_SetupAsync(
        SERCOM0,
        8000000u,
        baudrate,
        UART_TX_PAD,
        UART_RX_PAD,
        tx_buffer,
        rx_buffer
    );
}

void USART_Enable(void) {
    SERCOM_USART_Enable(SERCOM0);
}

void USART_WriteData(uint8_t* data, const uint8_t size) {
    SERCOM_USART_WriteData(SERCOM0, data, size);
}
