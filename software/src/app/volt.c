#include "app/volt.h"

uint16_t VOLT_CurrentVoltage = 0;

void VOLT_Initialize(void) {
    VOLT_CurrentVoltage = 0;
}

void VOLT_Update(void) {
    // TODO: implement ADC read and scaling
}

uint16_t VOLT_GetVoltage(void) {
    return VOLT_CurrentVoltage;
}
