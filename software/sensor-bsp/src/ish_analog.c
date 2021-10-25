#include "board/ish.h"
#include "board/ish_internal.h"

#include "hal/port.h"
#include "hal/ac.h"
#include "hal/vref.h"
#include "board/pins.h"

#include <avr/io.h>

const ac_configuration analog_settings = {
    .runstandy = false,
    .output_enable = false,
    .intmode = AC_INTERRUPT_MODE_BOTH,
    .hysmode = AC_HYSTERESIS_MODE_OFF
};

/**
 * @brief Initializes the input stage driver with the given configuration
 * 
 * @param configuration 
 */
void ish_setup_io() {
    ac_init(&analog_settings);

    port_setup_standby(ISH_INPUT_PIN);

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

uint8_t ish_get_event_channel(void) {
    return EVSYS__ASYNCCH0_AC0_OUT_gc;
}

bool ish_get_state(void) {
    return ac_get_state();
}
