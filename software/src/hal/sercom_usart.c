#include "hal/sercom_usart.h"

#include "atsamd21e18a.h"

#include <stdlib.h>

sercom_registers_t* get_peripheral(uint8_t sercom) {
    switch (sercom) {
        case SERCOM0: return SERCOM0_REGS;
    }
    return NULL;
}

ringbuffer8_t* buffers[4];

// goal for now is 9600-19200, 8N1, using interrupts
void SERCOM_USART_SetupAsync(uint8_t sercom, uint32_t clock,
                             uint32_t baudrate, uint8_t tx_pad,
                             uint8_t rx_pad, ringbuffer8_t* buffer) {

    sercom_registers_t* peripheral = get_peripheral(sercom);
    buffers[sercom] = buffer;

    peripheral->USART_INT.SERCOM_CTRLA = SERCOM_USART_INT_CTRLA_MODE_USART_INT_CLK | 
                                           SERCOM_USART_INT_CTRLA_RXPO(rx_pad) |
                                           SERCOM_USART_INT_CTRLA_TXPO(tx_pad) |
                                           SERCOM_USART_INT_CTRLA_DORD_Msk |    // is this needed?
                                           SERCOM_USART_INT_CTRLA_IBON_Msk |    // is this needed?
                                           SERCOM_USART_INT_CTRLA_FORM_USART_FRAME_NO_PARITY |
                                           SERCOM_USART_INT_CTRLA_SAMPR_8X_FRACTIONAL;

    uint32_t baud = (clock * 8) / (8 * baudrate);
    peripheral->USART_INT.SERCOM_BAUD = SERCOM_USART_INT_BAUD_FRAC_FP(baud % 8) |
                                          SERCOM_USART_INT_BAUD_FRAC_BAUD(baud / 8);

    peripheral->USART_INT.SERCOM_CTRLB = SERCOM_USART_INT_CTRLB_CHSIZE_8_BIT |
                                           SERCOM_USART_INT_CTRLB_SBMODE_1_BIT |
                                           //SERCOM_USART_INT_CTRLB_RXEN_Msk |
                                           SERCOM_USART_INT_CTRLB_TXEN_Msk;

    NVIC_EnableIRQ(SERCOM0_IRQn);
    NVIC_SetPriority(SERCOM0_IRQn, 2);
}

void SERCOM_USART_Enable(uint8_t sercom) {
    sercom_registers_t* peripheral = get_peripheral(sercom);
    /* Enable the UART after the configurations */
    peripheral->USART_INT.SERCOM_CTRLA |= SERCOM_USART_INT_CTRLA_ENABLE_Msk;

    /* Wait for sync */
    while(peripheral->USART_INT.SERCOM_SYNCBUSY & SERCOM_USART_INT_SYNCBUSY_ENABLE_Msk != 0);
}

// TODO: convert to accept all sercom channels
static inline void SERCOM0_USART_TX_INT_DISABLE() {
    SERCOM0_REGS->USART_INT.SERCOM_INTENCLR = SERCOM_USART_INT_INTENCLR_DRE_Msk;
}

// TODO: convert to accept all sercom channels
static inline void SERCOM0_USART_TX_INT_ENABLE() {
    SERCOM0_REGS->USART_INT.SERCOM_INTENSET = SERCOM_USART_INT_INTENSET_DRE_Msk;
}

volatile static bool writing = false; 

void SERCOM_USART_WriteData(uint8_t sercom, uint8_t* data, uint8_t size) {
    while(writing);

    for (int i = 0; i < size; i++) {
        ringbuffer8_write(buffers[sercom], data[i]);
    }

    writing = true;
    SERCOM0_USART_TX_INT_ENABLE();
    SERCOM0_REGS->USART_INT.SERCOM_DATA = ringbuffer8_read(buffers[0]);
}

// TODO: convert to accept all sercom channels
static inline void SERCOM0_USART_ISR_TX_Handler() {
    if (ringbuffer8_available(buffers[0])) {
        uint8_t data = ringbuffer8_read(buffers[0]);
        SERCOM0_REGS->USART_INT.SERCOM_DATA = data;
    }
    else {
        writing = false;
        SERCOM0_USART_TX_INT_DISABLE();
    }
}

void SERCOM0_Handler(void) {
    if(SERCOM0_REGS->USART_INT.SERCOM_INTENSET != 0)
    {
        /* Checks for data register empty flag */
        if((SERCOM0_REGS->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_DRE_Msk) && (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_DRE_Msk))
        {
            SERCOM0_USART_ISR_TX_Handler();
        }

        /* Checks for receive complete empty flag */
        if((SERCOM0_REGS->USART_INT.SERCOM_INTENSET & (SERCOM_USART_INT_INTENSET_RXC_Msk | SERCOM_USART_INT_INTENSET_RXBRK_Msk)) && (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & (SERCOM_USART_INT_INTFLAG_RXC_Msk | SERCOM_USART_INT_INTFLAG_RXBRK_Msk)))
        {
            //SERCOM0_USART_ISR_RX_Handler();
        }

        /* Checks for error flag */
        // if((SERCOM0_REGS->USART_INT.SERCOM_INTENSET & SERCOM_USART_INT_INTENSET_ERROR_Msk) && (SERCOM0_REGS->USART_INT.SERCOM_INTFLAG & SERCOM_USART_INT_INTFLAG_ERROR_Msk))
        // {
        //     SERCOM0_USART_ISR_ERR_Handler();
        // }
    }
}
