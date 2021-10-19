#include "app/init.h"
#include "app/config.h"

#include "hal/evsys.h"
#include "hal/wdt.h"
#include "hal/usart.h"
#include "hal/tcb.h"
#include "hal/sch.h"
#include "hal/rtc.h"

#include "board/board.h"
#include "board/ish.h"
#include "board/osh.h"

#include "sensor/pse.h"
#include "sensor/spe.h"

static const rtc_configuration rtc_config = {
    .clksel = RTC_CLKSEL_INT32K,
    .standby = false,
    .prescaler = RTC_PRESCALER_DIV1
};

void rtc_pit_handler(void) {
    sch_trigger();
}

void app_init() {
    wdt_init_normal(WDT_PERIOD_512CLK);

    board_init();

    /**
     * @brief 32768 Hz -> 0.000030517578125 s tick = 
     * 0.0009765625 s
     */
    rtc_init(&rtc_config);
    rtc_set_pit_period(RTC_PIT_PERIOD_CYC32);
    rtc_enable_pit_interrupt();
    rtc_enable();

    /** Usart config */
    usart_init_full_duplex((void*)0);
    usart_sync_setup_stdio();

    //pse_init(&pse_config);

    osh_output_on();

    ish_enable();

    evsys_user_async_select(0, 0x3);
}

void __attribute__((used, noinline)) _fatal(void) {
    while(1);
}
