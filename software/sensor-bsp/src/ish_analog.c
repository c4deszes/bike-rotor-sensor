#include "board/ish.h"

#include "hal/port.h"
#include "hal/ac.h"
#include "hal/vref.h"
#include "hal/tcb.h"
#include "hal/evsys.h"

#include <avr/io.h>
#include <stddef.h>

const uint8_t ish_pin = 7;
const uint8_t evsys_channel = 0;

const ac_configuration analog_settings = {
    .runstandy = false,
    .output_enable = false,
    .intmode = AC_INTERRUPT_MODE_BOTH,
    .hysmode = AC_HYSTERESIS_MODE_50MV
};

/**
 * @brief Initializes the input stage driver with the given configuration
 * 
 * @param configuration 
 */
void ish_init(ish_callback callback) {
    // setup analog comparator
    ac_init(&analog_settings);

    port_setup_standy(ish_pin);

    #ifdef BOARD_SENSOR_ANALOG_REF_EXTERNAL
    ac_setmux(false, AC_MUX_POSITIVE_P0, AC_MUX_NEGATIVE_N0);
    #elif BOARD_SENSOR_ANALOG_REF_0V55
    vref_select_ac0(VREF_VALUE_0V55);
    ac_setmux(false, AC_MUX_POSITIVE_P0, AC_MUX_NEGATIVE_VREF);
    #elif BOARD_SENSOR_ANALOG_REF_1V1
    vref_select_ac0(VREF_VALUE_1V1);
    ac_setmux(false, AC_MUX_POSITIVE_P0, AC_MUX_NEGATIVE_VREF);
    #elif BOARD_SENSOR_ANALOG_REF_1V5
    vref_select_ac0(VREF_VALUE_1V5);
    ac_setmux(false, AC_MUX_POSITIVE_P0, AC_MUX_NEGATIVE_VREF);
    #elif BOARD_SENSOR_ANALOG_REF_2V5
    vref_select_ac0(VREF_VALUE_2V5);
    ac_setmux(false, AC_MUX_POSITIVE_P0, AC_MUX_NEGATIVE_VREF);
    #elif BOARD_SENSOR_ANALOG_REF_4V3
    vref_select_ac0(VREF_VALUE_4V3);
    ac_setmux(false, AC_MUX_POSITIVE_P0, AC_MUX_NEGATIVE_VREF);
    #else
    #error "Unknown Analog Reference setting!"
    #endif

    ac_enable();
}

void ish_enable(void) {
    // 
    evsys_gen_async_select(evsys_channel, 0);
}

void ish_disable(void) {
    evsys_gen_async_disable(evsys_channel);
}

uint8_t ish_get_event_channel(void) {
    return evsys_channel;
}

bool ish_get_state(void) {
    // return ac state
    return ac_get_state();
}
