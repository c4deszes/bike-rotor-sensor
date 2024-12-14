#include "bsp/bmi088.h"

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

static int8_t BMI088_ReadAdapter(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
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

static int8_t BMI088_WriteAdapter(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
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

struct bmi08_dev BMI088_Device = {
    .read = BMI088_ReadAdapter,
    .write = BMI088_WriteAdapter,
    .delay_us = delayMicroseconds,
    .intf_ptr_accel = &bmi088_csa_pin,
    .intf_ptr_gyro = &bmi088_csg_pin,
    .intf = BMI08_SPI_INTF,
    .variant = BMI088_VARIANT,
    .read_write_len = 32
};

void BMI088_Initialize() {
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
