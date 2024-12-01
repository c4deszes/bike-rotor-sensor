#include "app/alt.h"

#include "hal/gpio.h"
#include "hal/sercom_spi.h"

#include "bsp/pinout.h"

#include "bmp5.h"
#include "bmp5_defs.h"
#include <math.h>

float ALT_Pressure;
float ALT_Altitude;

static float ALT_QNH = 1013.25f;

static const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = false
};

int8_t BMP581_ReadAdapter(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    GPIO_PinWrite(BMP581_SPI_CS_PORT, BMP581_SPI_CS_PIN, LOW);
    SERCOM_SPI_TransferByte(SERCOM1, reg_addr);  // Read register address (driver sets the R/W bit)
    
    // Dummy byte is included by the driver
    for (uint32_t i=0;i<len;i++) {
        uint8_t data = SERCOM_SPI_TransferByte(SERCOM1, 0xFF); // Read from slave

        reg_data[i] = data;
    }

    GPIO_PinWrite(BMP581_SPI_CS_PORT, BMP581_SPI_CS_PIN, HIGH);

    return BMP5_INTF_RET_SUCCESS;
}

int8_t BMP581_WriteAdapter(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    GPIO_PinWrite(BMP581_SPI_CS_PORT, BMP581_SPI_CS_PIN, LOW);
    SERCOM_SPI_TransferByte(SERCOM1, reg_addr);  // Read register address (driver sets the R/W bit)
    
    // Dummy byte is included by the driver
    for (uint32_t i=0;i<len;i++) {
        SERCOM_SPI_TransferByte(SERCOM1, reg_data[i]); // Read from slave
    }

    GPIO_PinWrite(BMP581_SPI_CS_PORT, BMP581_SPI_CS_PIN, HIGH);

    return BMP5_INTF_RET_SUCCESS;
}

static void delayMicroseconds( uint32_t usec, void* ptr )
{
  if ( usec == 0 )
  {
    return ;
  }

    // TODO: configurable F_CPU
  uint32_t n = usec * (48000000 / 1000000) / 3;
  __asm__ __volatile__(
    "1:              \n"
    "   sub %0, #1   \n" // substract 1 from %0 (n)
    "   bne 1b       \n" // if result is not 0 jump to 1
    : "+r" (n)           // '%0' is n variable with RW constraints
    :                    // no input
    :                    // no clobber
  );
  // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
  // https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html#Volatile
}

void ALT_Initialize() {
    GPIO_EnableFunction(BMP581_SPI_MOSI_PORT, BMP581_SPI_MOSI_PIN, BMP581_SPI_MOSI_PINMUX);
    GPIO_EnableFunction(BMP581_SPI_MISO_PORT, BMP581_SPI_MISO_PIN, BMP581_SPI_MISO_PINMUX);
    GPIO_EnableFunction(BMP581_SPI_SCK_PORT, BMP581_SPI_SCK_PIN, BMP581_SPI_SCK_PINMUX);

    GPIO_PinWrite(BMP581_SPI_CS_PORT, BMP581_SPI_CS_PIN, HIGH);
    GPIO_SetupPinOutput(BMP581_SPI_CS_PORT, BMP581_SPI_CS_PIN, &output);

    SERCOM_SPI_SetupMaster(SERCOM1, 8000000UL, 1000000UL,
                           sercom_spi_dataorder_msb,
                           sercom_spi_cpha_trailing, sercom_spi_cpol_idle_high,
                           SERCOM_SPI_MOSI_PAD0, SERCOM_SPI_MISO_PAD3);
    SERCOM_SPI_Enable(SERCOM1);
}

static bool bmp5_initialized = false;
int8_t bmp5_sensor_code;
static struct bmp5_dev bmp_dev = {
    .chip_id = BMP5_CHIP_ID_PRIM,
    .read = BMP581_ReadAdapter,
    .write = BMP581_WriteAdapter,
    .delay_us = delayMicroseconds,
    .intf = BMP5_SPI_INTF,
    .intf_ptr = NULL
};
static struct bmp5_osr_odr_press_config bmp_press_cfg = { 0 };
static struct bmp5_sensor_data sensor_data;

static void BMP5_SetupSensor() {
    bmp5_init(&bmp_dev);

    bmp5_sensor_code = bmp5_set_power_mode(BMP5_POWERMODE_STANDBY, &bmp_dev);

    if (bmp5_sensor_code != BMP5_OK) {
        return;
    }

    bmp5_sensor_code = bmp5_get_osr_odr_press_config(&bmp_press_cfg, &bmp_dev);
    if (bmp5_sensor_code != BMP5_OK) {
        return;
    }

    bmp_press_cfg.press_en = BMP5_ENABLE;
    // TODO: output data rate
    bmp5_sensor_code = bmp5_set_osr_odr_press_config(&bmp_press_cfg, &bmp_dev);
    if (bmp5_sensor_code != BMP5_OK) {
        return;
    }

    struct bmp5_iir_config set_iir_cfg;
    set_iir_cfg.set_iir_t = BMP5_IIR_FILTER_COEFF_1;
    set_iir_cfg.set_iir_p = BMP5_IIR_FILTER_COEFF_1;
    set_iir_cfg.shdw_set_iir_t = BMP5_ENABLE;
    set_iir_cfg.shdw_set_iir_p = BMP5_ENABLE;
    bmp5_sensor_code = bmp5_set_iir_config(&set_iir_cfg, &bmp_dev);
    if (bmp5_sensor_code != BMP5_OK) {
        return;
    }

    bmp5_sensor_code = bmp5_configure_interrupt(BMP5_PULSED, BMP5_ACTIVE_HIGH, BMP5_INTR_PUSH_PULL, BMP5_INTR_ENABLE, &bmp_dev);
    if (bmp5_sensor_code != BMP5_OK) {
        return;
    }

    struct bmp5_int_source_select int_source_select;
    int_source_select.drdy_en = BMP5_ENABLE;
    bmp5_sensor_code = bmp5_int_source_select(&int_source_select, &bmp_dev);
    if (bmp5_sensor_code != BMP5_OK) {
        return;
    }

    bmp5_sensor_code = bmp5_set_power_mode(BMP5_POWERMODE_CONTINOUS, &bmp_dev);
    if (bmp5_sensor_code != BMP5_OK) {
        return;
    }
}

void ALT_SetQNH(float qnh) {
    ALT_QNH = qnh;
}

float CalculateAltitude(float pressure, float qnh) {
    // Constants based on the ISA model
    const float seaLevelPressure = 1013.25; // hPa (standard sea level pressure)
    const float temperatureLapseRate = 0.0065; // K/m (standard lapse rate)
    const float standardTemperature = 288.15; // K (standard temperature at sea level)
    const float gravitationalAcceleration = 9.80665; // m/s²
    const float gasConstant = 287.05; // J/(kg·K) (specific gas constant for air)

    // Adjusted pressure based on QNH
    float adjustedPressure = pressure / qnh * seaLevelPressure;

    // Calculate altitude using the barometric formula (rearranged to avoid log functions)
    float altitude = 0.0;
    float currentPressure = adjustedPressure;

    // Iterative approximation (linearized for small pressure changes)
    while (currentPressure < seaLevelPressure) {
        altitude += 1.0; // Increment altitude in meters
        currentPressure *= (1.0 + (temperatureLapseRate / standardTemperature));
    }

    return altitude;
}


void ALT_Update(void) {
    if (!bmp5_initialized) {
        BMP5_SetupSensor();
        bmp5_initialized = true;
    }

    if (bmp5_sensor_code == BMP5_OK) {
        bmp5_get_sensor_data(&sensor_data, &bmp_press_cfg, &bmp_dev);
        ALT_Pressure = sensor_data.pressure;
        ALT_Altitude = CalculateAltitude(ALT_Pressure / 100, ALT_QNH);
    }
}