#include "bootloader/bootloader.h"
#include "bootloader/bootloader_internal.h"

#include "board/board.h"

#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>

#include "hal/cpu.h"
#include "board/pins.h"

FUSES = {
    .OSCCFG = FREQSEL_16MHZ_gc,
    .SYSCFG0 = CRCSRC_NOCRC_gc | RSTPINCFG_UPDI_gc,
    .SYSCFG1 = SUT_16MS_gc,
    .APPEND = 0x00,
    .BOOTEND = 0x02 /** < 2 * 256 bytes (512B) allocated to the bootloader */
};

__attribute__((noreturn)) void boot_startapp() {
    /** Shutdown the bootloader */
    cli();

    /** Lock BOOT section */
    NVMCTRL.CTRLB |= NVMCTRL_BOOTLOCK_bm;

    /** Relocate interrupt vectors to APP section */
    _PROTECTED_WRITE(CPUINT.CTRLA, CPUINT.CTRLA & ~(CPUINT_IVSEL_bm));

    /** Jump to application */
    __asm("jmp __application_start__");

    __builtin_unreachable();
}

boot_state_t boot_state __attribute__((__used__));

void boot_init(void) {
    /** Initialize the Watchdog */
    WDT.CTRLA = WDT_PERIOD_512CLK_gc;
    WDT.STATUS = WDT_LOCK_bm;

    /** Set system clock */
    uint8_t clk_setting = (CLKCTRL_PDIV_64X_gc << CLKCTRL_PDIV_gp) | CLKCTRL_PEN_bm;
    ccp_write_io(&(CLKCTRL.MCLKCTRLB), clk_setting);
    CLKCTRL.MCLKLOCK |= CLKCTRL_LOCKEN_bm;

    /** Relocate interrupt vectors to BOOT section */
    uint8_t cpuint_ctrla = CPUINT.CTRLA | CPUINT_IVSEL_bm;
    ccp_write_io(&(CPUINT.CTRLA), cpuint_ctrla);

    PORTB.DIRSET = _BV(5);
    PORTB.OUTSET = _BV(5);

    /** Uart portmux */
    PORTA.DIRSET = _BV(VCOM_TX_PIN);
    //PORTA.DIRCLR = _BV(VCOM_RX_PIN);
    PORTA.PIN2CTRL = PORT_PULLUPEN_bm;
    PORTMUX.CTRLB = PORTMUX_USART0_bm;

    /** Usart config */
    USART0.BAUD = ((64 * 250000ul) / (16 * 9600ul));
    USART0.CTRLA = USART_RXCIE_bm | USART_TXCIE_bm;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
    USART0.CTRLC = (USART_CMODE_ASYNCHRONOUS_gc << USART_CMODE_gp) | 
                   (USART_PMODE_DISABLED_gc << USART_PMODE_gp);

    sei();
}

/**
 * @brief Unexpected interrupt handler
 */
void __attribute__((used, noinline)) _fatal(void) {
    while(1);
}
