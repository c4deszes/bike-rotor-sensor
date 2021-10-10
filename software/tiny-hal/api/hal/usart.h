#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

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
} usart_full_duplex_configuration;

typedef struct {
    uint16_t baudrate;
    bool open_drain;
    usart_rx_handler rx_handler;
} usart_one_wire_configuration;

void usart_init_full_duplex(const usart_full_duplex_configuration* configuration);

void usart_init_one_wire(const usart_one_wire_configuration* configuration);

/**
 * @brief Disables the USART peripheral
 * 
 */
void usart_disable(void);

/**
 * @brief Writes a single byte synchronuously over the USART
 * 
 * @param data 
 */
void usart_sync_write(uint8_t data);

/**
 * @brief Reads a single byte synchronuously from USART port
 * 
 * @return uint8_t 
 */
uint8_t usart_sync_read();

void usart_sync_setup_stdio();

typedef struct {
    bool auto_flush;
    uint8_t* rx_buffer;
    uint8_t rx_buffer_size;
    uint8_t* tx_buffer;
    uint8_t tx_buffer_size;
} usart_async_configuration;

void usart_async_init(const usart_async_configuration configuration);

void usart_async_write(uint8_t data);

uint8_t usart_async_read();

uint8_t usart_async_available();
