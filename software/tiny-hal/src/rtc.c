#include "hal/rtc.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void rtc_init(const rtc_configuration* configuration) {
    // TODO: wait for sync
    uint8_t ctrla = 0;
    if (configuration->standby) {
        ctrla |= RTC_RUNSTDBY_bm;
    }
    ctrla |= configuration->prescaler;
    RTC.CTRLA = ctrla;
    RTC.CLKSEL = configuration->clksel;
}

void rtc_enable(void) {
    RTC.CTRLA |= RTC_RTCEN_bm;
}

void rtc_disable(void) {
    RTC.CTRLA &= ~(RTC_RTCEN_bm);
}

void rtc_set_period(uint16_t period) {
    RTC.PER = period;
}

void rtc_set_compare(uint16_t compare) {
    RTC.CMP = compare;
}

void rtc_set_pit_period(rtc_pit_period period) {
    RTC.PITCTRLA = ((period << RTC_PERIOD_gp) & RTC_PERIOD_gm) | RTC_PITEN_bm;

    while(RTC.PITSTATUS & RTC_CTRLABUSY_bm);
}

void rtc_enable_pit_interrupt() {
    RTC.PITINTCTRL = RTC_PI_bm;
}

ISR(RTC_PIT_vect) {
    rtc_pit_handler();
    RTC.PITINTFLAGS = RTC_PI_bm;
}
