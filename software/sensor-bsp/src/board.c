/**
 * @file board.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "board/board.h"

#include "hal/sys.h"
#include "hal/clkctrl.h"
#include "board/osh.h"
#include "board/ish.h"
#include <stddef.h>

const uint32_t CPU_FREQUENCY = 20000000UL;

/**
 * @brief Initializes the clock settings
 * 
 * @note Initial settings depend on the fuse settings
 */
void clock_init(void) {
    clkctrl_set_clock(CLKCTRL_CLOCK_SELECT_OSC20M);
    clkctrl_set_prescaler(CLKCTRL_PRESCALER_DIV16);
    clkctrl_lock();
}

void board_init() {
    sys_init(CPU_FREQUENCY);

    clock_init();

    osh_init();

    ish_init(NULL);
}
