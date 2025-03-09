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

    EVSYS_REGS->EVSYS_CTRL = EVSYS_CTRL_SWRST_Msk;
    while((EVSYS_REGS->EVSYS_CTRL & EVSYS_CTRL_SWRST_Msk) == EVSYS_CTRL_SWRST_Msk)
    {
        /* Wait for sync */
    }

    EIC_REGS->EIC_CTRL |= EIC_CTRL_SWRST_Msk;

    while((EIC_REGS->EIC_STATUS & EIC_STATUS_SYNCBUSY_Msk) == EIC_STATUS_SYNCBUSY_Msk)
    {
        /* Wait for sync */
    }

    EIC_REGS->EIC_EVCTRL = EIC_EVCTRL_EXTINTEO7_Msk | EIC_EVCTRL_EXTINTEO10_Msk | EIC_EVCTRL_EXTINTEO15_Msk;
    EIC_REGS->EIC_CONFIG[0] = EIC_CONFIG_SENSE7_HIGH;
    EIC_REGS->EIC_CONFIG[1] = EIC_CONFIG_SENSE2_HIGH | EIC_CONFIG_SENSE7_HIGH;

    /* Enable External Interrupt Controller */
    EIC_REGS->EIC_CTRL |= EIC_CTRL_ENABLE_Msk;

    while ((EIC_REGS->EIC_STATUS & EIC_STATUS_SYNCBUSY_Msk) == EIC_STATUS_SYNCBUSY_Msk) {
        /* Wait for sync */
    }

    /* Event system configuration */
    evgens[0] = EVSYS_AcquireGeneratorChannel();
    EVSYS_ConfigureGenerator(evgens[0], NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_7);        // Input 1 - Front wheel

    evgens[1] = EVSYS_AcquireGeneratorChannel();
    EVSYS_ConfigureGenerator(evgens[1], NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_10);       // Input 2 - Rear wheel

    evgens[2] = EVSYS_AcquireGeneratorChannel();
    EVSYS_ConfigureGenerator(evgens[2], NO_EVT_OUTPUT, ASYNCHRONOUS, EVENT_ID_GEN_EIC_EXTINT_15);       // Input 3 - Crankset

    /* Timer configuration */
    TC_SetupCapture(TC3, tc_prescaler_div1);
    TC3_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_PRESCALER_DIV4;
    EVSYS_ConfigureUser(EVENT_ID_USER_TC3_EVU, evgens[0]);

    TC_SetupCapture(TC4, tc_prescaler_div1);
    TC4_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_PRESCALER_DIV4;
    EVSYS_ConfigureUser(EVENT_ID_USER_TC4_EVU, evgens[1]);

    TC_SetupCapture(TC5, tc_prescaler_div1);
    TC5_REGS->COUNT16.TC_CTRLA |= TC_CTRLA_PRESCALER_DIV4;
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

    OSH_PhySetChannelStatus(2, osh_phy_channel_state_ok);

    // TODO: check error pin of MAX9921
    // issue1: error pin will be low because IN2 is open
    // need to run diagnostics occasionally to determine if IN1 is faulty too
    // need to disable timer during diagnostics
    // if (GPIO_PinRead(MAX9921_ERROR_PORT, MAX9921_ERROR_PIN) == HIGH) {
    //     GPIO_PinWrite(MAX9921_DIAG_PORT, MAX9921_DIAG_PIN, LOW);
    //     OSH_PhySetChannelStatus(2, osh_phy_channel_state_ok);
    // }
    // else {
    //     GPIO_PinWrite(MAX9921_DIAG_PORT, MAX9921_DIAG_PIN, HIGH);
    //     if (GPIO_PinRead(MAX9921_INPUT3_PORT, MAX9921_INPUT3_PIN) == HIGH) {
    //         OSH_PhySetChannelStatus(2, osh_phy_channel_state_short);
    //     }
    //     else {
    //         OSH_PhySetChannelStatus(2, osh_phy_channel_state_open);
    //     }
    // }
}

#define TIMER_HANDLER(handler, regs, id) void handler(void) { \
    if ((regs->COUNT16.TC_INTFLAG & TC_INTFLAG_ERR_Msk) == TC_INTFLAG_ERR_Msk) { \
        \
    } \
    else if ((regs->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF_Msk) == TC_INTFLAG_OVF_Msk) { \
        SENSOR_Process(id, 0, 0xFFFFul); \
    } \
    else if ((regs->COUNT16.TC_INTFLAG & TC_INTFLAG_MC1_Msk) == TC_INTFLAG_MC1_Msk) { \
        uint32_t cc0 = regs->COUNT16.TC_CC[0]; \
        uint32_t cc1 = regs->COUNT16.TC_CC[1]; \
        if (cc0 > cc1) { \
            SENSOR_Process(id, cc1, cc0); \
        } \
        else { \
            SENSOR_Process(id, cc0, cc1); \
        } \
    } \
    regs->COUNT16.TC_INTFLAG = TC_INTFLAG_Msk; \
}

TIMER_HANDLER(TC3_Handler, TC3_REGS, 0)
TIMER_HANDLER(TC4_Handler, TC4_REGS, 1)
TIMER_HANDLER(TC5_Handler, TC5_REGS, 2)
