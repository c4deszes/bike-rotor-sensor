#include "app/alt.h"
#include "bsp/bmp581.h"
#include "app/config.h"

#include "common/swtimer.h"

#include "bmp5.h"
#include "bmp5_defs.h"

float ALT_Pressure;
float ALT_Altitude;

static float ALT_QNH = 1020.00f;

static swtimer_t* ALT_SensorTimer = NULL;
static bool ALT_SensorInitialized = false;
static int8_t ALT_BMP5_InitCode = BMP5_OK;
static struct bmp5_osr_odr_press_config ALT_BMP5_RateConfig = { 0 };
static struct bmp5_iir_config ALT_BMP5_IIRConfig = {
    .set_iir_t = BMP5_IIR_FILTER_COEFF_1,
    .set_iir_p = BMP5_IIR_FILTER_COEFF_1,
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
    ALT_SensorTimer = SWTIMER_Create();
    SWTIMER_Setup(ALT_SensorTimer, ALT_SENSOR_STARTUP_TIME);
}

float CalculateAltitude(float pressure, float qnh) {
    // Constants based on the ISA model
    const float seaLevelPressure = 1013.25; // hPa
    const float temperatureLapseRate = 0.0065; // K/m
    const float standardTemperature = 288.15; // K
    const float gasConstantOverGravity = 8434.5; // m (R/g for air)

    // Convert inputs to hPa
    float adjustedPressure = pressure / qnh * seaLevelPressure; // Convert Pa to hPa

    // Direct calculation of altitude
    float altitude = gasConstantOverGravity * 
                     (1.0 - (adjustedPressure / seaLevelPressure) * 
                     (standardTemperature / (standardTemperature - temperatureLapseRate * 100.0)));

    return altitude;
}

void ALT_SetQNH(float qnh) {
    ALT_QNH = qnh;
}

void ALT_Update(void) {
    if (!ALT_SensorInitialized && SWTIMER_Elapsed(ALT_SensorTimer)) {
        ALT_SetupSensor();
        ALT_SensorInitialized = true;
    }

    if (ALT_SensorInitialized && ALT_BMP5_InitCode == BMP5_OK) {
        bmp5_get_sensor_data(&ALT_BMP5_SensorData, &ALT_BMP5_RateConfig, &BMP581_Device);
        ALT_Pressure = ALT_BMP5_SensorData.pressure;
        ALT_Altitude = CalculateAltitude(ALT_Pressure / 100, ALT_QNH);
    }
}
