#include "app/init.h"
#include "app/config.h"

#include "hal/wdt.h"
#include "hal/usart.h"
#include "hal/tca.h"
#include "hal/tcb.h"
#include "hal/sch.h"
#include "board/board.h"
#include "sensor/pse.h"
#include "sensor/spe.h"

#include "hal/portmux.h"

static const tca_configuration tca_config = {
    .clksel = TCA_CLKSEL_CLKDIV1
};

void tca_overflow_handler() {
    sch_trigger();
}

void app_init() {
    wdt_init_normal(WDT_PERIOD_512CLK);

    board_init();

    /** Scheduler configuration */
    tca_init(&tca_config);
    tca_set_period(250);
    tca_enable_overflow_interrupt(tca_overflow_handler);
    tca_enable();

    /**
     * CLK_PER = 250 000
     * 262ms period
     */
    tcb_init((void*)0);
    tcb_enable_interrupt((void*)0);
    tcb_enable();

    /** Usart config */
    usart_init_full_duplex((void*)0);
    usart_sync_setup_stdio();

    //pse_init(&pse_config);
}

void __attribute__((used, noinline)) _fatal(void) {
    while(1);
}
