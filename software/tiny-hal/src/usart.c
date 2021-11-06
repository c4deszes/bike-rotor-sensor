#include "hal/usart.h"

#include "hal/sys.h"

#include <avr/io.h>
#include <avr/interrupt.h>

inline uint16_t usart_calc_baud_async(uint32_t clk_per, uint32_t baudrate) {
    return (64 * clk_per) / (16 * baudrate);
}

void usart_init_one_wire(const usart_one_wire_configuration* configuration) {
    USART0.BAUD = usart_calc_baud_async(sys_get_cpu_frequency(), configuration->baudrate);
    USART0.CTRLA = USART_LBME_bm;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_ODME_bm;
    USART0.CTRLC = (configuration->com_mode << USART_CMODE_gp) | 
                   (configuration->parity_mode << USART_PMODE_gp);
}

void usart_init_full_duplex(const usart_full_duplex_configuration* configuration) {
    USART0.BAUD = usart_calc_baud_async(sys_get_cpu_frequency(), configuration->baudrate);
    USART0.CTRLA = 0;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
    USART0.CTRLC = (configuration->com_mode << USART_CMODE_gp) |
                   (configuration->parity_mode << USART_PMODE_gp);
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
}

uint8_t usart_sync_read() {
    /** Wait for reception */
    while ((USART0.STATUS & USART_RXCIF_bm) == 0);

    return USART0.RXDATAL;
}

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

    uint8_t* tx_buffer;
    uint8_t tx_buffer_size;
    uint8_t tx_buffer_head;
    uint8_t tx_buffer_tail;
} usart_async_state;

void usart_async_write(uint8_t data) {
    usart_async_state.tx_buffer[usart_async_state.tx_buffer_head++] = data;
}

uint8_t usart_async_read() {
    if (usart_async_state.rx_buffer_head == usart_async_state.rx_buffer_tail) {
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
   uint8_t received = USART0.RXDATAL;
}
