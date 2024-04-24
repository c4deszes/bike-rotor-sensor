#include "hal/gpio.h"
#include "hal/evsys.h"

#include "bsp/osh_phy.h"
#include "bsp/osh_phy_internal.h"
#include "bsp/board.h"
#include "bsp/pinout.h"

#include "atsamd21e18a.h"

static const gpio_pin_input_configuration input = {
    .pull = FLOATING,
    .sample = CONTINUOUS
};

static const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = false
};

static uint8_t evgens[2] = {0xFF, 0xFF};

void OSH_PhyInit_Impl(void) {
    GPIO_SetupPinInput(AP22802_CH1_INPUT_PORT, AP22802_CH1_INPUT_PIN, &input);
    GPIO_EnableFunction(AP22802_CH1_INPUT_PORT, AP22802_CH1_INPUT_PIN, AP22802_CH1_INPUT_PINMUX);

    GPIO_SetupPinInput(AP22802_CH2_INPUT_PORT, AP22802_CH2_INPUT_PIN, &input);
    GPIO_EnableFunction(AP22802_CH2_INPUT_PORT, AP22802_CH2_INPUT_PIN, AP22802_CH2_INPUT_PINMUX);

    GPIO_SetupPinInput(AP22802_CH1_OC_PORT, AP22802_CH1_OC_PIN, &input);
    GPIO_SetupPinInput(AP22802_CH2_OC_PORT, AP22802_CH2_OC_PIN, &input);

    GPIO_SetupPinOutput(AP22802_CH1_EN_PORT, AP22802_CH1_EN_PIN, &output);
    GPIO_SetupPinOutput(AP22802_CH2_EN_PORT, AP22802_CH2_EN_PIN, &output);

    //bsp_eic_config.config[4].event = true;
    //bsp_eic_config.config[4].sense = INPUT_SENSE_BOTH;
    // Evsys channel 0 as EIC EXTINT4 event source
    evgens[0] = EVSYS_AcquireGeneratorChannel();
    EVSYS_ConfigureGenerator(evgens[0], NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_4);

    //bsp_eic_config.config[5].event = true;
    //bsp_eic_config.config[5].sense = INPUT_SENSE_BOTH;
    // Evsys channel 1 as EIC EXTINT5 event source
    evgens[1] = EVSYS_AcquireGeneratorChannel();
    EVSYS_ConfigureGenerator(evgens[1], NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_5);
}

uint8_t OSH_PhyGetEventGenerator(uint8_t channel) {
    return evgens[channel];
}

void OSH_PhyUpdate(void) {
    // TODO: could we detect open status?

    if (GPIO_PinRead(AP22802_CH1_OC_PORT, AP22802_CH1_OC_PIN) == HIGH) {
        OSH_PhySetChannelStatus(0, osh_phy_channel_state_ok);
    }
    else {
        OSH_PhySetChannelStatus(0, osh_phy_channel_state_short);
    }

    if (GPIO_PinRead(AP22802_CH2_OC_PORT, AP22802_CH2_OC_PIN) == HIGH) {
        OSH_PhySetChannelStatus(1, osh_phy_channel_state_ok);
    }
    else {
        OSH_PhySetChannelStatus(1, osh_phy_channel_state_short);
    }
}

void OSH_PhyTurnOn_Impl(void) {
    GPIO_PinWrite(AP22802_CH1_EN_PORT, AP22802_CH1_EN_PIN, HIGH);
    GPIO_PinWrite(AP22802_CH2_EN_PORT, AP22802_CH2_EN_PIN, HIGH);
}

void OSH_PhyTurnOff_Impl(void) {
    GPIO_PinWrite(AP22802_CH1_EN_PORT, AP22802_CH1_EN_PIN, LOW);
    GPIO_PinWrite(AP22802_CH2_EN_PORT, AP22802_CH2_EN_PIN, LOW);
}

