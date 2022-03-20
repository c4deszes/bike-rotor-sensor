#include "app/init.h"
#include "app/config.h"

#include "hal/adc.h"
#include "hal/evsys.h"
#include "hal/wdt.h"
#include "hal/usart.h"
#include "hal/sch.h"
#include "hal/rtc.h"
#include "hal/vref.h"

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

static const usart_full_duplex_configuration usart_phy_config = {
    .com_mode = USART_COM_MODE_ASYNC,
    .baudrate = 9600,
    .parity_mode = USART_PARITY_MODE_NONE
};

static sensor_configuration_t sensor_config = {
    .pulse_per_rotation = SENSOR_PULSE_PER_ROTATION,
    .index_threshold = SENSOR_INDEX_THRESHOLD
};

static adc_configuration adc_config = {
    .resolution = ADC_RESOLUTION_8BIT,
    .runstandby = false,
    .sampling = ADC_SAMPLING_ACC4
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

    adc_init(&adc_config);
    vref_select_adc0(VREF_VALUE_1V1);

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
    usart_init_full_duplex(&usart_phy_config);

    /** Initialize sensor data processing components */
    pse_init(&sensor_config);
    dsa_init(&sensor_config);
    spe_init(&sensor_config);

    /** Enables sensor hardware */
    osh_output_on();
    ish_enable();
}

/**
 * @brief Unexpected interrupt handler
 */
void __attribute__((used, noinline)) _fatal(void) {
    while(1);
}
