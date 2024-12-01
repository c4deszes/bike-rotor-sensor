#include "bsp/osh_phy.h"
#include "hal/evsys.h"
#include "hal/tc.h"
#include "bsp/sensor.h"
#include "hal/gpio.h"
#include "app/config.h"

#include "bsp/pinout.h"

static const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = false
};

static const gpio_pin_input_configuration input_pullup = {
    .pull = PULLUP,
    .sample = CONTINUOUS
};

static const gpio_pin_input_configuration input = {
    .pull = FLOATING,
    .sample = CONTINUOUS
};

static uint32_t accumulates[3];
static osh_phy_channel_state_t phy_states[3];
static uint8_t evgens[3] = {0xFF, 0xFF, 0xFF};

void OSH_PhyInit(void) {
    GPIO_SetupPinInput(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN, &input_pullup);
    GPIO_EnableFunction(MAX9621_INPUT1_PORT, MAX9621_INPUT1_PIN, MAX9621_INPUT1_PINMUX);
    GPIO_SetupPinInput(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN, &input_pullup);
    GPIO_EnableFunction(MAX9621_INPUT2_PORT, MAX9621_INPUT2_PIN, MAX9621_INPUT2_PINMUX);
    GPIO_SetupPinInput(MAX9621_INPUT1_DIAG_PORT, MAX9621_INPUT1_DIAG_PIN, &input);          // TODO: connect to ADC
    GPIO_SetupPinInput(MAX9621_INPUT2_DIAG_PORT, MAX9621_INPUT2_DIAG_PIN, &input);          // TODO: connect to ADC
    GPIO_SetupPinOutput(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, &output);

    GPIO_SetupPinInput(MAX9921_INPUT3_PORT, MAX9921_INPUT3_PIN, &input_pullup);
    GPIO_EnableFunction(MAX9921_INPUT3_PORT, MAX9921_INPUT3_PIN, MAX9921_INPUT3_PINMUX);
    GPIO_SetupPinOutput(MAX9921_ENABLE_PORT, MAX9921_ENABLE_PIN, &output);
    GPIO_SetupPinOutput(MAX9921_DIAG_PORT, MAX9921_DIAG_PIN, &output);
    GPIO_SetupPinInput(MAX9921_ERROR_PORT, MAX9921_ERROR_PIN, &input_pullup);
    // because OUT2 is not connected we can only determine short circuits, or imply open circuit

    /* Event system configuration */
    evgens[0] = EVSYS_AcquireGeneratorChannel();
    EVSYS_ConfigureGenerator(evgens[0], NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_7);

    evgens[1] = EVSYS_AcquireGeneratorChannel();
    EVSYS_ConfigureGenerator(evgens[1], NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_10);

    evgens[2] = EVSYS_AcquireGeneratorChannel();
    EVSYS_ConfigureGenerator(evgens[1], NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_15);

    /* Timer configuration */
    TC_SetupCapture(TC3, tc_prescaler_div1);
    EVSYS_ConfigureUser(EVENT_ID_USER_TC3_EVU, evgens[0]);

    TC_SetupCapture(TC4, tc_prescaler_div1);
    EVSYS_ConfigureUser(EVENT_ID_USER_TC4_EVU, evgens[1]);

    TC_SetupCapture(TC5, tc_prescaler_div1);
    EVSYS_ConfigureUser(EVENT_ID_USER_TC5_EVU, evgens[2]);
}

void OSH_PhySetChannelStatus(uint8_t channel, osh_phy_channel_state_t status) {
    phy_states[channel] = status;
}

osh_phy_channel_state_t OSH_PhyGetChannelState(uint8_t channel) {
    return phy_states[channel];
}

void OSH_PhyTurnOn(void) {
    GPIO_PinWrite(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, HIGH);
    GPIO_PinWrite(MAX9921_ENABLE_PORT, MAX9921_ENABLE_PIN, HIGH);

    TC_Start(TC3);
    TC_Start(TC4);
    TC_Start(TC5);

    for (uint8_t i = 0; i < 3; i++) {
        OSH_PhySetChannelStatus(i, osh_phy_channel_state_ok);
    }
}

void OSH_PhyTurnOff(void) {
    TC_Stop(TC3);
    TC_Stop(TC4);
    TC_Stop(TC5);

    GPIO_PinWrite(MAX9621_SLEEP_PORT, MAX9621_SLEEP_PIN, LOW);
    GPIO_PinWrite(MAX9921_ENABLE_PORT, MAX9921_ENABLE_PIN, LOW);

    for (uint8_t i = 0; i < 3; i++) {
        OSH_PhySetChannelStatus(i, osh_phy_channel_state_off);
    }
}

void OSH_PhyUpdate(void) {
    // if DIAG1 is Low (0) and Input1 is Low -> Ground shorted
    // if DIAG2 is Low (1) and Input2 is High -> Open

    // if DIAG1 is High (above 1V) then Input1 is connected
    // if DIAG2 is High (above 1V) then Input2 is connected
    // TODO: this may not work as the diag1 high level is a little bit below the Vhol
    if (GPIO_PinRead(MAX9621_INPUT1_DIAG_PORT, MAX9621_INPUT1_DIAG_PIN) == HIGH) {
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
    if (GPIO_PinRead(MAX9621_INPUT2_DIAG_PORT, MAX9621_INPUT2_DIAG_PIN) == HIGH) {
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

    // TODO: check error pin of MAX9921
    OSH_PhySetChannelStatus(2, osh_phy_channel_state_ok);
}

// TODO: move interrupt handlers to HAL
void TC3_Handler(void) {
    if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
        // TODO: notify the sensor about the error
        accumulates[0] = 0;
    }
    else if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        // TODO: accumulate the impulses up to the sensor limits
        accumulates[0] += 0xFFFF; // or 0xFFFF
        if (accumulates[0] >= 10000000ul) { // 10s
            SENSOR_Process(0, 0, accumulates[0]);
            accumulates[0] = 0;
        }
    }
    else if ((TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        uint16_t cc0 = TC3_REGS->COUNT16.TC_CC[0];
        uint16_t cc1 = TC3_REGS->COUNT16.TC_CC[1];
        SENSOR_Process(0, cc0, cc1 + accumulates[0]);
        accumulates[0] = 0;
    }

    TC3_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}

// TODO: move interrupt handlers to HAL
void TC4_Handler(void) {
    if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
        // TODO: notify the sensor about the error, reset accumulates
        accumulates[1] = 0;
    }
    else if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        // TODO: accumulate the impulses up to the sensor limits
        accumulates[1] += 0xFFFF; // or 0xFFFF
        if (accumulates[1] >= 10000000ul) { // 10s
            SENSOR_Process(1, 0, accumulates[1]);
            accumulates[1] = 0;
        }
    }
    else if ((TC4_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        uint16_t cc0 = TC4_REGS->COUNT16.TC_CC[0];
        uint16_t cc1 = TC4_REGS->COUNT16.TC_CC[1];
        // TODO: question, to which term do we add the accumulate? was stuck in high or low?
        SENSOR_Process(1, cc0, cc1 + accumulates[1]);
        accumulates[1] = 0;
    }

    TC4_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}

void TC5_Handler(void) {
    if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) {
        // TODO: notify the sensor about the error, reset accumulates
        accumulates[2] = 0;
    }
    else if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) {
        // TODO: accumulate the impulses up to the sensor limits
        accumulates[2] += 0xFFFF; // or 0xFFFF
        if (accumulates[2] >= 10000000ul) { // 10s
            SENSOR_Process(2, 0, accumulates[2]);
            accumulates[2] = 0;
        }
    }
    else if ((TC5_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) {
        uint16_t cc0 = TC5_REGS->COUNT16.TC_CC[0];
        uint16_t cc1 = TC5_REGS->COUNT16.TC_CC[1];
        // TODO: question, to which term do we add the accumulate? was stuck in high or low?
        SENSOR_Process(2, cc0, cc1 + accumulates[2]);
        accumulates[2] = 0;
    }

    TC5_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk;
}
