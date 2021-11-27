#include "bootloader/bootloader.h"
#include "bootloader/bootloader_internal.h"
#include "bootloader/memory.h"

#include "bootloader/flash_util.h"
#include "bootloader/usart_drv.h"

#include <stddef.h>

#include <avr/io.h>

#include "hal/cpu.h"
#include "board/pins.h"

uint16_t* flash_ptr = NULL;

/**
 * Flash command: [LENGTH] 0x32 [ADDR] [DATA]
 * Erase command: 0x03 0x33 [SECTION]
 */
void command_handle(uint8_t cmd, uint8_t* data, uint8_t length) {
    if (cmd == 0x33) {
        PORTA.OUTCLR = _BV(5);
        // Do erase
        boot_erase_section(data[0]);
    }
    else if (cmd == 0x32) {
        // Do flash
        boot_write_section();
        uint8_t i = 0;
        while(i < length) {
            *flash_ptr = *((uint16_t*)(data + i));
            flash_ptr++;
            i++;
        }
    }
}

enum {
    WAITING,
    LENGTH_RECEIVED,
    RECEIVING,
    MESSAGE_RECEIVED
} com_state = WAITING;
uint8_t com_length = 0;
uint8_t* message_begin = NULL;
uint8_t* message_end = NULL;

void usart_handle(uint8_t* data) {
    if (com_state == WAITING) {
        com_length = *data;
        message_begin = data;
        com_state = LENGTH_RECEIVED;
    }
    else if (com_state == LENGTH_RECEIVED || com_state == RECEIVING) {
        com_state = RECEIVING;
        com_length--;

        if (com_length == 0) {
            message_end = data;
            com_state = MESSAGE_RECEIVED;

            command_handle(message_begin[1], message_begin + 2, message_begin[0]-2);

            com_state = WAITING;
        }
    }
}

void boot_enter(void) {
    flash_ptr = (uint16_t*)__application_start__;

    while(1) {
        if (usart_available() > 0) {
            uint8_t* data = usart_read();
            usart_handle(data);
        }
    }
}
