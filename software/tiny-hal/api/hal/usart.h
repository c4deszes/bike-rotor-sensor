#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    USART_COM_MODE_ASYNC = 0x00,
    USART_COM_MODE_SYNC = 0x01
} usart_com_mode;

typedef enum {
    USART_PARITY_MODE_NONE = 0x00,
    USART_PARITY_MODE_EVEN = 0x02,
    USART_PARITY_MODE_ODD = 0x03
} usart_parity_mode;

typedef enum {
    USART_RX_ERROR_BUFFER_OVERFLOW,
    USART_RX_ERROR_FRAME_ERROR,
    USART_RX_ERROR_PARITY_ERROR,
    USART_RX_ERROR_AUTOBAUD_ERROR
} usart_rx_error;

typedef void (*usart_rx_error_handler)(usart_rx_error error);

typedef void (*usart_rx_handler)(uint8_t data);

typedef void (*usart_tx_handler)(uint8_t* data);

typedef struct {
    uint16_t baudrate;
    bool open_drain;
    usart_rx_handler rx_handler;
    usart_tx_handler tx_handler;
} usart_one_wire_configuration;

void usart_init_one_wire(const usart_one_wire_configuration* configuration);

void usart_rx_enable(void);

void usart_rx_disable(void);

void usart_tx_enable(void);

void usart_tx_disable(void);
