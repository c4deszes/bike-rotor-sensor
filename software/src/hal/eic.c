/**
 * @file eic.c
 * @author Balazs Eszes
 * @brief External Interrupt Controller implementation
 */
#include <stdint.h>

#include "hal/eic.h"

#include "atsamd21e18a.h"

// eic_callback eic_handles[16];
// eic_callback nmi_handle;

void EIC_Initialize(const eic_configuration* conf) {
    /* Enable Main clock for EIC */
    //MCLK_REGS->MCLK_APBAMASK |= MCLK_APBAMASK_EIC_Msk;

    /* Reset the EIC peripheral */
    EIC_REGS->EIC_CTRL |= EIC_CTRL_SWRST_Msk;

    while((EIC_REGS->EIC_STATUS & EIC_STATUS_SYNCBUSY_Msk) == EIC_STATUS_SYNCBUSY_Msk)
    {
        /* Wait for sync */
    }

    // uint32_t evctrl = 0;
    // uint32_t config0 = 0;
    // uint32_t config1 = 0;
    // for (uint8_t i = 0; i < 16; i++) {
    //     if (conf->config[i].event) {
    //         evctrl |= (1 << i);
    //     }
    //     uint8_t setting = 0;
    //     if (conf->config[i].filter) {
    //         setting |= 0b1000;
    //     }
    //     setting |= conf->config[i].sense;
    //     if (i <= 7) {
    //         config0 |= (setting) << (i * 4);
    //     }
    //     else if (i >= 8 && i <= 15) {
    //         config1 |= (setting) << (i * 4);
    //     }
    // }

    //EIC_REGS->EIC_EVCTRL = evctrl;
    //EIC_REGS->EIC_CONFIG[0] = config0;
    //EIC_REGS->EIC_CONFIG[1] = config1;

    EIC_REGS->EIC_EVCTRL = EIC_EVCTRL_EXTINTEO4_Msk | EIC_EVCTRL_EXTINTEO5_Msk;
    EIC_REGS->EIC_CONFIG[0] = EIC_CONFIG_SENSE4_HIGH | EIC_CONFIG_SENSE5_HIGH;
    //EIC_REGS->EIC_CONFIG[1] = config1;

    /* Enable External Interrupt Controller */
    EIC_REGS->EIC_CTRL |= EIC_CTRL_ENABLE_Msk;

    while ((EIC_REGS->EIC_STATUS & EIC_STATUS_SYNCBUSY_Msk) == EIC_STATUS_SYNCBUSY_Msk) {
        /* Wait for sync */
    }
}

void EIC_EnableInterrupt(uint8_t pin) {
    EIC_REGS->EIC_INTENSET = (1 << pin);
}

void EIC_DisableInterrupt(uint8_t pin) {
    EIC_REGS->EIC_INTENCLR = (1 << pin);
}

void NonMaskableInt_Handler(void) {
    EIC_REGS->EIC_NMIFLAG = EIC_NMIFLAG_NMI_Msk;
}

// #define EIC_EXTINT_HANDLER(INDEX) \
//     void EIC_EXTINT_##INDEX##_Handler(void) { \
//         EIC_REGS->EIC_INTFLAG = (1 << INDEX); \
//     }

// EIC_EXTINT_HANDLER(0)
// EIC_EXTINT_HANDLER(1)
// EIC_EXTINT_HANDLER(2)
// EIC_EXTINT_HANDLER(3)
// EIC_EXTINT_HANDLER(4)
// EIC_EXTINT_HANDLER(5)
// EIC_EXTINT_HANDLER(6)
// EIC_EXTINT_HANDLER(7)
