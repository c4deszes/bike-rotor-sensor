#include "hal/usart.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void usart_init_one_wire(const usart_one_wire_configuration* configuration) {

}

void usart_init_full_duplex(const usart_full_duplex_configuration* configuration) {
    /**
     * @brief 
     * 64 * 250000 / (9600 * 16)
     */
    USART0.BAUD = 104;
    //USART0.CTRLC = 0;
    //USART0.CTRLA = USART_RXCIE_bm;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
}

void usart_disable(void) {
    #ifdef __AVR_ATtiny416__
    /**
     * @brief See errata 2.11.1 TXD Pin Override not released
     */
    USART0.CTRLB &= ~(USART_TXEN_bm);
    USART0.CTRLB &= ~(USART_RXEN_bm);
    #else
    USART0.CTRLB &= ~(USART_RXEN_bm | USART_TXEN_bm);
    #endif
}

void usart_sync_write(uint8_t data) {
    USART0.TXDATAL = data;

    /** Wait for the data register to be empty */
    while ((USART0.STATUS & USART_DREIF_bm) == 0);

    return 0;
}

uint8_t usart_sync_read() {
    /** Wait for reception */
    while ((USART0.STATUS & USART_RXCIF_bm) == 0);

    return USART0.RXDATAL;
}

/**
 * @brief Adapter to connect stdout to the synchronuous USART driver
 * 
 * @param c 
 * @param stream 
 * @return int 
 */
static int _stdio_usart_sync_write(char c, FILE* stream) {
    usart_sync_write((uint8_t)c);
    return 0;
}

static FILE usart_sync_stdout = FDEV_SETUP_STREAM(_stdio_usart_sync_write, NULL, _FDEV_SETUP_WRITE);

void usart_sync_setup_stdio() {
    stdout = &usart_sync_stdout;
}

struct {
    uint8_t* rx_buffer;
    uint8_t rx_buffer_size;
    uint8_t rx_buffer_head;
    uint8_t rx_buffer_tail;
    uint8_t rx_buffer_index;

    uint8_t* tx_buffer;
    uint8_t tx_buffer_size;
    uint8_t tx_buffer_head;
    uint8_t tx_buffer_tail;
    uint8_t tx_buffer_index;
} usart_async_state;

void usart_async_write(uint8_t data) {
    usart_async_state.tx_buffer[usart_async_state.tx_buffer_head++] = data;
}

uint8_t usart_async_read() {
    if (usart_async_state.rx_buffer_head == usart_async_state.rx_buffer_index) {
        return -1;
    }
    uint8_t data = usart_async_state.rx_buffer[usart_async_state.rx_buffer_tail];
    usart_async_state.rx_buffer_tail = (usart_async_state.rx_buffer_tail + 1) % usart_async_state.rx_buffer_size;
    return data;
}

uint8_t usart_async_available() {
    return ((unsigned int)(usart_async_state.rx_buffer_size
                            + usart_async_state.rx_buffer_head
                            - usart_async_state.rx_buffer_tail)) % usart_async_state.rx_buffer_size;
}

ISR(USART0_RXC_vect) {
   //received = USART0.RXDATAL;
   PORTB.OUTTGL = _BV(5);
}
