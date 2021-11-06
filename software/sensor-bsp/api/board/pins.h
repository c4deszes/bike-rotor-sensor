#pragma once

#define ISH_INPUT_PIN 7

#define OSH_OUTPUT_PIN 2

#if defined(BOARD_TYPE_XPLAINED)
    #define VCOM_TX_PIN 1
    #define VCOM_RX_PIN 2
#elif defined(BOARD_TYPE_STANDARD)
    #define VCOM_TX_PIN 1
    #define VCOM_RX_PIN 1
#else
    #error "Cannot determine VCOM pin settings!"
#endif
