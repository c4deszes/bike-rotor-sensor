#pragma once

#include <stdint.h>

void ccp_write_spm(volatile uint8_t* address, uint8_t data);

/**
 * @brief Writes a change protected register
 * @example ccp_write_io(&CLKCTRL.MCLKCTRLA, 0)
 * 
 * @param address Address to write
 * @param data Data to write
 */
void ccp_write_io(volatile uint8_t* address, uint8_t data);
