#if !defined(HAL_SERCOM_USART_)
#define HAL_SERCOM_USART_

#include "common/ringbuffer.h"

#define SERCOM0 0
#define SERCOM1 1
#define SERCOM2 2
#define SERCOM3 3
#define SERCOM4 4
#define SERCOM5 5

#define SERCOM_USART_TX_PAD0 0
#define SERCOM_USART_TX_PAD2 1

#define SERCOM_USART_RX_PAD0 0
#define SERCOM_USART_RX_PAD1 1
#define SERCOM_USART_RX_PAD2 2
#define SERCOM_USART_RX_PAD3 3

// params: clock in, baudrate
void SERCOM_USART_SetupAsync(uint8_t sercom, uint32_t clock,
                             uint32_t baudrate, uint8_t tx_pad,
                             uint8_t rx_pad, ringbuffer8_t* buffer);

void SERCOM_USART_Enable(uint8_t sercom);

void SERCOM_USART_WriteData(uint8_t sercom, uint8_t* data, uint8_t size);

//uint8_t SERCOM_USART_Read(uint8_t sercom);

//size_t SERCOM_USART_ReadCountGet(uint8_t sercom);

#endif // HAL_SERCOM_USART_
