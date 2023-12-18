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
    GPIO_SetupPinInput(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN, &input);
    GPIO_EnableFunction(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN, MAX9621_INPUT1_PINMUX);

    GPIO_SetupPinInput(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN, &input);
    GPIO_EnableFunction(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN, MAX9621_INPUT2_PINMUX);

    GPIO_SetupPinInput(MAX9621_DIAG1_PORT, MAX9621_DIAG1_PIN, &input);
    GPIO_SetupPinInput(MAX9621_DIAG2_PORT, MAX9621_DIAG2_PIN, &input);

    GPIO_SetupPinOutput(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, &output);

    // TODO: configure DIAG1/2 as ADC pins

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
    // if DIAG1 is Low (0) and Input1 is Low -> Ground shorted
    // if DIAG2 is Low (1) and Input2 is High -> Open

    // if DIAG1 is High (above 1V) then Input1 is connected
    // if DIAG2 is High (above 1V) then Input2 is connected
    // TODO: this may not work as the diag1 high level is a little bit below the Vhol
    if (GPIO_PinRead(MAX9621_DIAG1_PORT, MAX9621_DIAG1_PIN) == HIGH) {
        OSH_PhySetChannelStatus(0, osh_phy_channel_state_ok);
    }
    else {
        if(GPIO_PinRead(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN) == HIGH) {
            OSH_PhySetChannelStatus(0, osh_phy_channel_state_short);
        }
        else {
            OSH_PhySetChannelStatus(0, osh_phy_channel_state_open);
        }
    }

    // TODO: this may not work as the diag1 high level is a little bit below the Vhol
    if (GPIO_PinRead(MAX9621_DIAG2_PORT, MAX9621_DIAG2_PIN) == HIGH) {
        OSH_PhySetChannelStatus(1, osh_phy_channel_state_ok);
    }
    else {
        if(GPIO_PinRead(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN) == HIGH) {
            OSH_PhySetChannelStatus(1, osh_phy_channel_state_short);
        }
        else {
            OSH_PhySetChannelStatus(1, osh_phy_channel_state_open);
        }
    }
}

void OSH_PhyTurnOn_Impl(void) {
    GPIO_PinWrite(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, HIGH);
}

void OSH_PhyTurnOff_Impl(void) {
    GPIO_PinWrite(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, LOW);
}

