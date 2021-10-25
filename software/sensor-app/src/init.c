#include "app/init.h"
#include "app/config.h"

#include "hal/evsys.h"
#include "hal/wdt.h"
#include "hal/usart.h"
#include "hal/sch.h"
#include "hal/rtc.h"

#include "board/board.h"
#include "board/ish.h"
#include "board/osh.h"

#include "sensor/dsa.h"
#include "sensor/pse.h"
#include "sensor/spe.h"

static const rtc_configuration rtc_config = {
    .clksel = RTC_CLKSEL_INT32K,
    .standby = false,
    .prescaler = RTC_PRESCALER_DIV1
};

static const dsa_configuration dsa_config = {
    .pulse_per_rotation = SENSOR_PULSE_PER_ROTATION
};

static const pse_configuration pse_config = {
    .pulse_per_rotation = SENSOR_PULSE_PER_ROTATION,
    .index_threshold = SENSOR_INDEX_THRESHOLD
};

static const spe_configuration spe_config = {
    .pulse_per_rotation = SENSOR_PULSE_PER_ROTATION
};

/**
 * @brief RTC Periodic Interrupt handler configured trigger the scheduler every 1 millisecond
 */
void rtc_pit_handler(void) {
    sch_trigger();
}

void app_init() {
    /** Initialize the Watchdog */
    wdt_init_normal(WDT_PERIOD_512CLK);

    /** Initialize system and board peripherals */
    board_init();

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

    /** Usart config */
    usart_init_full_duplex((void*)0);
    usart_sync_setup_stdio();

    /** Initialize sensor data processing components */
    pse_init(&pse_config);
    dsa_init(&dsa_config);
    spe_init(&spe_config);

    /** Enables sensor hardware */
    osh_output_on();
    ish_enable();
}

void __attribute__((used, noinline)) _fatal(void) {
    while(1);
}
