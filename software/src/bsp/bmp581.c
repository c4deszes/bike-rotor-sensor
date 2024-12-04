#include "bsp/bmp581.h"
#include "bsp/pinout.h"

#include "bmp5.h"
#include "bmp5_defs.h"

#include "hal/gpio.h"
#include "hal/sercom_spi.h"

static const gpio_pin_output_configuration output = {
    .drive = NORMAL,
    .input = false
};

static int8_t BMP581_ReadAdapter(uint8_t reg_addr, uint8_t *reg_data, uint32_t len, void *intf_ptr)
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

static int8_t BMP581_WriteAdapter(uint8_t reg_addr, const uint8_t *reg_data, uint32_t len, void *intf_ptr)
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

struct bmp5_dev BMP581_Device = {
    .chip_id = BMP5_CHIP_ID_PRIM,
    .read = BMP581_ReadAdapter,
    .write = BMP581_WriteAdapter,
    .delay_us = delayMicroseconds,
    .intf = BMP5_SPI_INTF,
    .intf_ptr = NULL
};

void BMP581_Initialize() {
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
