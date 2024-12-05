#include "app/alt.h"
#include "bsp/bmp581.h"
#include "app/config.h"

#include "common/swtimer.h"

#include "bmp5.h"
#include "bmp5_defs.h"

static ALT_Status_t ALT_Status = ALT_Status_NotAvailable;
static uint32_t ALT_Pressure = 0;
static int16_t ALT_Altitude = 0;
static uint16_t ALT_Temperature = 0;
static uint32_t ALT_QNH = ALT_SENSOR_DEFAULT_QNH;

static swtimer_t* ALT_SensorTimer = NULL;
static bool ALT_SensorInitialized = false;
static int8_t ALT_BMP5_InitCode = BMP5_OK;
static struct bmp5_osr_odr_press_config ALT_BMP5_RateConfig = { 0 };
static struct bmp5_iir_config ALT_BMP5_IIRConfig = {
    .set_iir_t = BMP5_IIR_FILTER_COEFF_1,   // TODO: choose filter coefficient
    .set_iir_p = BMP5_IIR_FILTER_COEFF_1,   // TODO: choose filter coefficient
    .shdw_set_iir_t = BMP5_ENABLE,
    .shdw_set_iir_p = BMP5_ENABLE
};
static struct bmp5_sensor_data ALT_BMP5_SensorData = { 0 };

static void ALT_SetupSensor() {
    bmp5_init(&BMP581_Device);

    ALT_BMP5_InitCode = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, &BMP581_Device);

    if (ALT_BMP5_InitCode != BMP5_OK) {
        return;
    }

    ALT_BMP5_InitCode = bmp5_get_osr_odr_press_config(&ALT_BMP5_RateConfig, &BMP581_Device);
    if (ALT_BMP5_InitCode != BMP5_OK) {
        return;
    }

    ALT_BMP5_RateConfig.press_en = BMP5_ENABLE;
    // TODO: output data rate

    ALT_BMP5_InitCode = bmp5_set_osr_odr_press_config(&ALT_BMP5_RateConfig, &BMP581_Device);
    if (ALT_BMP5_InitCode != BMP5_OK) {
        return;
    }

    ALT_BMP5_InitCode = bmp5_set_iir_config(&ALT_BMP5_IIRConfig, &BMP581_Device);
    if (ALT_BMP5_InitCode != BMP5_OK) {
        return;
    }

    ALT_BMP5_InitCode = bmp5_set_power_mode(BMP5_POWERMODE_CONTINOUS, &BMP581_Device);
    if (ALT_BMP5_InitCode != BMP5_OK) {
        return;
    }
}

void ALT_Initialize(void) {
    ALT_Status = ALT_Status_NotAvailable;
    ALT_SensorTimer = SWTIMER_Create();
    SWTIMER_Setup(ALT_SensorTimer, ALT_SENSOR_STARTUP_TIME);
}

ALT_Status_t ALT_GetStatus(void) {
    return ALT_Status;
}

int16_t ALT_GetAltitude(void) {
    return ALT_Altitude;
}

uint32_t ALT_GetPressure(void) {
    return ALT_Pressure;
}

void ALT_SetQNH(uint32_t qnh) {
    ALT_QNH = qnh;
}

#define M 289644uLL        // molar mass of Earth's air in µg/mol (scaled by 10^6)
#define g 980665uLL        // gravitational acceleration in µm/s² (scaled by 10^5)
#define R 831432uLL        // universal gas constant in µJ/(mol·K) (scaled by 10^5)

// Fixed-point scaling factor
#define SCALE 1000  // Scaling factor for easier integer calculations

// Function to calculate altitude
int32_t CalcAltitude(int32_t a, int32_t k, int32_t i) {
    if ((a / i) < (101325L / 22632L)) {
        int32_t d = -65;  // Lapse rate in µK/m (scaled by 100)
        int64_t temp_ratio = (i * SCALE) / a; // Scaled pressure ratio
        int64_t alt = (k * (SCALE - temp_ratio)) / -d;
        return alt;
    } else if ((a / i) < (101325L / 5474L)) {
        int32_t e = 11000;  // Base altitude for this layer in meters
        int32_t b = k - 715;  // Adjusted temperature in µK (scaled by 100)
        int64_t pressure_ratio = (i * SCALE) / a;
        int64_t f = (R * b * pressure_ratio) / (g * M);
        return e + f;
    }
    return -1;  // Return -1 if conditions are out of bounds
}

void ALT_Update(void) {
    if (!ALT_SensorInitialized && SWTIMER_Elapsed(ALT_SensorTimer)) {
        ALT_SetupSensor();

        if (ALT_BMP5_InitCode == BMP5_OK) {
            ALT_Status = ALT_Status_Ok;
        } else {
            ALT_Status = ALT_Status_Error;
        }

        ALT_SensorInitialized = true;
    }

    if (ALT_SensorInitialized && ALT_BMP5_InitCode == BMP5_OK) {
        int8_t read_status = bmp5_get_sensor_data(&ALT_BMP5_SensorData, &ALT_BMP5_RateConfig, &BMP581_Device);
        
        if (read_status == BMP5_OK) {
            // TODO: Convert altitude from ALT_BMP5_SensorData.pressure
            // TODO: Calculate altitude from pressure and QNH
            ALT_Pressure = ALT_BMP5_SensorData.pressure / 100;
            ALT_Temperature = ALT_BMP5_SensorData.temperature / 100 + 273;
            ALT_Altitude = CalcAltitude(ALT_QNH, ALT_Temperature, ALT_Pressure);
            ALT_Status = ALT_Status_Ok;
        }
        else {
            // TODO: increase error counter, disable operation after a number of errors
            ALT_Status = ALT_Status_Error;
        }
    }
}
