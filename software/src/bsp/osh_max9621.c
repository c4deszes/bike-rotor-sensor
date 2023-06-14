#include "hal/gpio.h"
#include "hal/evsys.h"

#include "bsp/osh_phy.h"
#include "bsp/board.h"
#include "bsp/config.h"

#include "atsamd21e18a.h"

const gpio_pin_input_configuration input = {
    .pull = FLOATING,
    .sample = CONTINUOUS
};

const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = false
};

void osh_phy_init_internal(void) {
    GPIO_SetupPinInput(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN, &input);
    GPIO_EnableFunction(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN, 0);

    GPIO_SetupPinInput(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN, &input);
    GPIO_EnableFunction(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN, 0);

    GPIO_SetupPinOutput(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, &output);

    // TODO: configure ADC pins

    bsp_eic_config.config[4].event = true;
    bsp_eic_config.config[4].sense = INPUT_SENSE_BOTH;
    // Evsys channel 0 as EIC EXTINT4 event source
    EVSYS_ConfigureGenerator(1, NO_EVT_OUTPUT, ASYNCHRONOUS, 0x10);

    bsp_eic_config.config[5].event = true;
    bsp_eic_config.config[5].sense = INPUT_SENSE_BOTH;
    // Evsys channel 1 as EIC EXTINT5 event source
    EVSYS_ConfigureGenerator(2, NO_EVT_OUTPUT, ASYNCHRONOUS, 0x11);
}

uint8_t osh_phy_get_evgen(uint8_t channel) {
    return 1 + channel;
}

void osh_phy_update(void) {

}

osh_channel_status_t osh_phy_channel_status(uint8_t channel) {
    return osh_channel_status_ok;
}

bool osh_phy_has_separate_channel_control(void) {
    return false;
}

void osh_phy_turn_on(void) {
    GPIO_PinWrite(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, HIGH);
}

void osh_phy_turn_off(void) {
    GPIO_PinWrite(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, LOW);
}

