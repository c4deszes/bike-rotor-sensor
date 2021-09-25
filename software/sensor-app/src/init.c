#include "app/init.h"

#include "app/init.h"

#include "app/config.h"

#include "hal/wdt.h"
#include "hal/usart.h"
#include "board/board.h"
#include "sensor/pse.h"
#include "sensor/spe.h"

static const pse_configuration pse_config = {
    .pulse_per_rotation = 64,
    .index_threshold = 200,
    .min_pulse_width = 10
};

static const usart_one_wire_configuration usart_config = {
    .baudrate = 9600,
    .rx_handler = 0
};

void app_init() {
    //wdt_init_normal(WDT_WINDOW_512CLK);

    board_init();

    //pse_init(&pse_config);
}

void __attribute__((used, noinline)) _fatal(void) {
    while(1);
}
