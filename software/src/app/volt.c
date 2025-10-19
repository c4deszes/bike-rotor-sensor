#include "app/volt.h"
#include "hal/gpio.h"
#include "hal/adc.h"

#include "sam.h"

#define VOLT_CALIB_SLOPE 1099 // Calibration slope for voltage measurement
#define VOLT_MEAS_REF 33 // Reference voltage
#define VOLT_MEAS_RES 1024

uint16_t VOLT_MeasuredVoltage = 0;
uint16_t VOLT_RawAdcValue = 0;

void VOLT_Initialize(void) {
    VOLT_MeasuredVoltage = 0;

    GPIO_EnableFunction(PORT_GROUP_A, 8, MUX_PA08B_ADC_AIN16);
    ADC_SetupSingleShot();
}

void VOLT_Update(void) {
    VOLT_RawAdcValue = ADC_ReadSync(16);

    VOLT_MeasuredVoltage = (VOLT_RawAdcValue * VOLT_MEAS_REF * VOLT_CALIB_SLOPE) / (VOLT_MEAS_RES); // Convert ADC value to millivolts
}

uint16_t VOLT_GetVoltage(void) {
    return VOLT_MeasuredVoltage;
}
