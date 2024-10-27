#include "hal/sercom_usart.h"
#include "bsp/pinout.h"

#include "bsp/usart.h"

static const gpio_pin_input_configuration input = {
    .pull = FLOATING,
    .sample = CONTINUOUS
};

static const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = ONDEMAND
};

void USART_Initialize(uint32_t baudrate, ringbuffer8_t* tx_buffer, ringbuffer8_t* rx_buffer) {
    GPIO_EnableFunction(UART_TX_PORT, UART_TX_PIN, UART_TX_PINMUX);
    GPIO_EnableFunction(UART_RX_PORT, UART_RX_PIN, UART_RX_PINMUX);
    GPIO_SetupPinOutput(UART_CS_PORT, UART_CS_PIN, &output);
    GPIO_PinWrite(UART_CS_PORT, UART_CS_PIN, HIGH);

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

void SERCOM0_Interrupt(void) {
    SERCOM_USART_InterruptHandler(SERCOM0);
}

void USART_Enable(void) {
    SERCOM_USART_Enable(SERCOM0);
}

void USART_WriteData(uint8_t* data, const uint8_t size) {
    SERCOM_USART_WriteData(SERCOM0, data, size);
}

uint16_t USART_Available(void) {
    return SERCOM_USART_Available(SERCOM0);
}

uint8_t USART_Read(void) {
    return SERCOM_USART_Read(SERCOM0);
}

void USART_FlushOutput(void) {
    SERCOM_USART_FlushOutput(SERCOM0);
}
