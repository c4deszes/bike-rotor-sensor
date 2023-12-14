#include "app/sch.h"
#include "hal/wdt.h"

#include <stdint.h>

#include "app/comm.h"
#include "app/sec.h"
#include "app/spm.h"
#include "app/iet.h"
#include "app/itpms.h"

#include "hal/gpio.h"
#include "common/swtimer.h"

#include "bsp/usart.h"

// void SCH_Task100us(void) {
//     //COMM_UpdatePhy();
// }

void SCH_Task1ms(void) {
    SWTIMER_Update1ms();

    SEC_Update();

    COMM_UpdatePhy();

    //SPM_Update();
}

//const char* text = "A1B2C3D4E5F6G7H8I9J0K1L2M3N4O5P6Q7R89T0U1V2W3X4Y5Z6!78901234\r\n";
//uint16_t counter = 0;

void SCH_Task10ms_A(void) {
    // counter++;
    // if (counter >= 100) {
    //     USART_WriteData(text, 63);
    //     USART_FlushOutput();
    //     counter = 0;
    // }
    //WDT_Acknowledge();

    //IET_Update();

    //ITPMS_Update();

    //COMM_UpdateSignals();
    //GPIO_PinToggle(PORT_GROUP_A, 18);
}
