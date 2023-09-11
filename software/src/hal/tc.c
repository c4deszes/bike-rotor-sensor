#include "hal/tc.h"
#include "atsamd21e18a.h"

// void TC_InitializeCapture(tc_registers_t* peripheral, tc_capture_config* config) {
//     peripheral->COUNT16.TC_CTRLA = TC_CTRLA_MODE_COUNT16 | config->prescaler;
//     peripheral->COUNT16.TC_CTRLC = TC_CTRLC_CPTEN0_Msk | TC_CTRLC_CPTEN1_Msk;
//     peripheral->COUNT16.TC_EVCTRL = TC_EVCTRL_EVACT_PPW | TC_EVCTRL_TCEI_Msk;
//     peripheral->COUNT16.TC_INTENSET = TC_INTENSET_OVF_Msk | TC_INTENSET_ERR_Msk | TC_INTENSET_MC0_Msk;
// }
