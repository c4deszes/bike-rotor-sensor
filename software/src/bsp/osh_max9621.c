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

void OSH_PhyInit_Impl(void) {
    GPIO_SetupPinInput(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN, &input);
    GPIO_EnableFunction(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN, PORT_PMUX_PMUXE_A);

    GPIO_SetupPinInput(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN, &input);
    GPIO_EnableFunction(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN, PORT_PMUX_PMUXE_A);

    GPIO_SetupPinOutput(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, &output);

    // TODO: configure DIAG1/2 as ADC pins

    //bsp_eic_config.config[4].event = true;
    //bsp_eic_config.config[4].sense = INPUT_SENSE_BOTH;
    // Evsys channel 0 as EIC EXTINT4 event source
    EVSYS_ConfigureGenerator(1, NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_4);

    //bsp_eic_config.config[5].event = true;
    //bsp_eic_config.config[5].sense = INPUT_SENSE_BOTH;
    // Evsys channel 1 as EIC EXTINT5 event source
    EVSYS_ConfigureGenerator(2, NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_5);
}

uint8_t OSH_PhyGetEventGenerator(uint8_t channel) {
    return 1 + channel;
}

void OSH_PhyUpdate(void) {
    // TODO: regularly poll DIAG1 and DIAG2 pins
    // if DIAG1 is Low (0) and Input1 is Low -> Ground shorted
    // if DIAG2 is Low (1) and Input2 is High -> Open
}

void OSH_PhyTurnOn_Impl(void) {
    GPIO_PinWrite(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, HIGH);
}

void OSH_PhyTurnOff_Impl(void) {
    GPIO_PinWrite(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, LOW);
}

