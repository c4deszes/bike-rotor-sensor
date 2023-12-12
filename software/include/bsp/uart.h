#if !defined(BSP_UART_H_)
#define BSP_UART_H_

#include <stdint.h>
#include "common/ringbuffer.h"

// known internally: clock, pads, pins
void USART_Initialize(uint32_t baudrate, ringbuffer8_t* buffer);

void USART_Enable(void);

//uint16_t USART_Available(void);

//uint8_t USART_Read(void);

//void USART_Write(uint8_t data);

void USART_WriteData(uint8_t* data, const uint8_t size);

#endif // BSP_UART_H_
