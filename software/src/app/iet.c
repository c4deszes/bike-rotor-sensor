#include "app/iet.h"

#include "hal/gpio.h"
#include "hal/sercom_spi.h"

#include "bsp/pinout.h"

#include "bmi08.h"
#include "bmi08_defs.h"
#include "bmi08x.h"

static const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = false
};

static uint8_t bmi088_csa_pin = BMI088_SPI_CS_ACC_PIN;
static uint8_t bmi088_csg_pin = BMI088_SPI_CS_GYRO_PIN;

int8_t BMI088_ReadAdapter(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    GPIO_PinWrite(BMI088_SPI_CS_ACC_PORT, *((uint8_t*)intf_ptr), LOW);
    SERCOM_SPI_TransferByte(SERCOM3, reg_addr);  // Read register address (driver sets the R/W bit)
    
    // Dummy byte is included by the driver
    for (uint32_t i=0;i<len;i++) {
        uint8_t data = SERCOM_SPI_TransferByte(SERCOM3, 0xFF); // Read from slave

        reg_data[i] = data;
    }

    GPIO_PinWrite(BMI088_SPI_CS_ACC_PORT, *((uint8_t*)intf_ptr), HIGH);

    return BMI08_INTF_RET_SUCCESS;
}

int8_t BMI088_WriteAdapter(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
{
    GPIO_PinWrite(BMI088_SPI_CS_ACC_PORT, *((uint8_t*)intf_ptr), LOW);
    SERCOM_SPI_TransferByte(SERCOM3, reg_addr);  // Read register address (driver sets the R/W bit)
    
    // Dummy byte is included by the driver
    for (uint32_t i=0;i<len;i++) {
        SERCOM_SPI_TransferByte(SERCOM3, reg_data[i]); // Read from slave
    }

    GPIO_PinWrite(BMI088_SPI_CS_ACC_PORT, *((uint8_t*)intf_ptr), HIGH);

    return BMI08_INTF_RET_SUCCESS;
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

void IET_Initialize(void) {
    GPIO_EnableFunction(BMI088_SPI_MISO_PORT, BMI088_SPI_MISO_PIN, BMI088_SPI_MISO_PINMUX);
    GPIO_EnableFunction(BMI088_SPI_MOSI_PORT, BMI088_SPI_MOSI_PIN, BMI088_SPI_MOSI_PINMUX);
    GPIO_EnableFunction(BMI088_SPI_SCK_PORT, BMI088_SPI_SCK_PIN, BMI088_SPI_SCK_PINMUX);

    GPIO_PinWrite(BMI088_SPI_CS_ACC_PORT, BMI088_SPI_CS_ACC_PIN, HIGH);
    GPIO_SetupPinOutput(BMI088_SPI_CS_ACC_PORT, BMI088_SPI_CS_ACC_PIN, &output);

    GPIO_PinWrite(BMI088_SPI_CS_GYRO_PORT, BMI088_SPI_CS_GYRO_PIN, HIGH);
    GPIO_SetupPinOutput(BMI088_SPI_CS_GYRO_PORT, BMI088_SPI_CS_GYRO_PIN, &output);

    SERCOM_SPI_SetupMaster(SERCOM3, 8000000UL, 1000000UL,
                           sercom_spi_dataorder_msb,
                           sercom_spi_cpha_trailing, sercom_spi_cpol_idle_high,
                           SERCOM_SPI_MOSI_PAD0, SERCOM_SPI_MISO_PAD3);
    SERCOM_SPI_Enable(SERCOM3);
}

static bool bmi08_initialized = false;
static int8_t bmi08_sensor_code;

struct bmi08_dev bmi08dev;

/*! @brief variable to hold the bmi08 accel data */
struct bmi08_sensor_data bmi08_accel;

/*! @brief variable to hold the bmi08 gyro data */
struct bmi08_sensor_data bmi08_gyro;

/*! bmi08 accel int config */
struct bmi08_accel_int_channel_cfg accel_int_config;

/*! bmi08 gyro int config */
struct bmi08_gyro_int_channel_cfg gyro_int_config;

void BMI088_SetupSensor() {
    bmi08dev.read = BMI088_ReadAdapter;
    bmi08dev.write = BMI088_WriteAdapter;
    bmi08dev.delay_us = delayMicroseconds;
    bmi08dev.intf_ptr_accel = &bmi088_csa_pin;
    bmi08dev.intf_ptr_gyro = &bmi088_csg_pin;
    bmi08dev.intf = BMI08_SPI_INTF;
    bmi08dev.variant = BMI088_VARIANT;
    bmi08dev.read_write_len = 32;

    bmi08_sensor_code = bmi08xa_init(&bmi08dev);

    if (bmi08_sensor_code != BMI08_OK) {
        return;
    }

    bmi08_sensor_code = bmi08g_init(&bmi08dev);

    if (bmi08_sensor_code != BMI08_OK) {
        return;
    }

    bmi08_sensor_code = bmi08a_load_config_file(&bmi08dev);

    if (bmi08_sensor_code != BMI08_OK) {
        return;
    }

    bmi08dev.accel_cfg.odr = BMI08_ACCEL_ODR_100_HZ;
    bmi08dev.accel_cfg.range = BMI088_ACCEL_RANGE_6G;
    bmi08dev.accel_cfg.power = BMI08_ACCEL_PM_ACTIVE; /*user_accel_power_modes[user_bmi088_accel_low_power]; */
    bmi08dev.accel_cfg.bw = BMI08_ACCEL_BW_NORMAL; /* Bandwidth and OSR are same */

    bmi08_sensor_code = bmi08a_set_power_mode(&bmi08dev);
    if (bmi08_sensor_code != BMI08_OK) {
        return;
    }

    bmi08_sensor_code = bmi08xa_set_meas_conf(&bmi08dev);
    if (bmi08_sensor_code != BMI08_OK) {
        return;
    }

    bmi08dev.gyro_cfg.odr = BMI08_GYRO_BW_12_ODR_100_HZ;
    bmi08dev.gyro_cfg.range = BMI08_GYRO_RANGE_125_DPS;
    bmi08dev.gyro_cfg.bw = BMI08_GYRO_BW_12_ODR_100_HZ;
    bmi08dev.gyro_cfg.power = BMI08_GYRO_PM_NORMAL;

    bmi08_sensor_code = bmi08g_set_power_mode(&bmi08dev);
    if (bmi08_sensor_code != BMI08_OK) {
        return;
    }

    bmi08_sensor_code = bmi08g_set_meas_conf(&bmi08dev);
    if (bmi08_sensor_code != BMI08_OK) {
        return;
    }
}

static int8_t bmi08_read_status;

void IET_Update(void) {
    if (!bmi08_initialized) {
        BMI088_SetupSensor();
        bmi08_initialized = true;
    }

    if (bmi08_initialized && bmi08_sensor_code == BMI08_OK) {
        bmi08_read_status = bmi08a_get_data(&bmi08_accel, &bmi08dev);

        bmi08_read_status = bmi08g_get_data(&bmi08_gyro, &bmi08dev);
    }
}
