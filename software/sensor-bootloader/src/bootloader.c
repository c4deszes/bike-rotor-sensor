#include "bootloader/bootloader.h"
#include "bootloader/bootloader_internal.h"

#include "hal/evsys.h"
#include "hal/cpuint.h"
#include "hal/wdt.h"
#include "hal/nvmctrl.h"
#include "hal/usart.h"
#include "hal/sch.h"
#include "hal/sys.h"
#include "hal/rtc.h"
#include "hal/rstctrl.h"

#include "board/board.h"

#include <avr/io.h>
#include <avr/fuse.h>

FUSES = {
    .OSCCFG = FREQSEL_16MHZ_gc,
    .SYSCFG0 = CRCSRC_NOCRC_gc | RSTPINCFG_UPDI_gc,
    .SYSCFG1 = SUT_16MS_gc,
    .APPEND = 0x00,
    .BOOTEND = 0x02 /** < 2 * 256 bytes (512B) allocated to the bootloader */
};

static const rtc_configuration rtc_config = {
    .clksel = RTC_CLKSEL_INT32K,
    .standby = false,
    .prescaler = RTC_PRESCALER_DIV1
};

static const usart_full_duplex_configuration usart_phy_config = {
    .com_mode = USART_COM_MODE_ASYNC,
    .baudrate = 9600,
    .parity_mode = USART_PARITY_MODE_NONE
};

/**
 * @brief RTC Periodic Interrupt handler configured trigger the scheduler every 1 millisecond
 */
void rtc_pit_handler(void) {
    sch_trigger();
}

__attribute__((noreturn)) void boot_startapp() {
    /** Shutdown the bootloader */
    sys_disable_interrupts();
    rtc_disable();

    /** Lock BOOT section */
    nvmctrl_bootlock();

    /** Relocate interrupt vectors to APP section */
    cpuint_ivsel(CPUINT_IVSEL_APP);

    /** Jump to application */
    __asm("jmp __application_start__");

    __builtin_unreachable();
}

boot_state_t boot_state __attribute__((__used__));

void boot_init(void) {
    PORTB.DIRSET = _BV(5);
    PORTB.OUTCLR = _BV(5);

    /** Initialize the Watchdog */
    wdt_init_normal(WDT_PERIOD_512CLK);

    /** Set system clock */
    board_clock_init();

    /** Relocate interrupt vectors to BOOT section */
    cpuint_ivsel(CPUINT_IVSEL_BOOT);

    // if (rstctrl_get_cause() == RSTCTRCL_CAUSE_POWER_ON) {
    //     boot_state.boot_count = 0;
    // }
    // else {
    //     boot_state.boot_count++;
    // }

    /**
     * @note Calculations:
     *  CLK_RTC = 32768 Hz
     *  TimePerTick = 1 / CLK_RTC = 0.000030517578125s
     *  PitPeriod = round(0.001ms / TimePerTick) = 32
     */
    rtc_init(&rtc_config);
    rtc_set_pit_period(RTC_PIT_PERIOD_CYC32);
    rtc_enable_pit_interrupt();
    rtc_enable();

    /** Uart portmux */
    //board_vcom_init();

    /** Usart config */
    //usart_init_full_duplex(&usart_phy_config);

    //boot_startapp();
}

/**
 * @brief Unexpected interrupt handler
 */
void __attribute__((used, noinline)) _fatal(void) {
    while(1);
}
