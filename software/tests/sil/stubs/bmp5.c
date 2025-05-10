#include "bmp5.h"

/* Mocking Bosch BMP5xx driver */
int8_t bmp5_init(struct bmp5_dev *dev)
{
    return BMP5_OK;
}

int8_t bmp5_set_power_mode(enum bmp5_powermode powermode, struct bmp5_dev *dev)
{
    return BMP5_OK;
}

int8_t bmp5_get_osr_odr_press_config(struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev)
{
    return BMP5_OK;
}

int8_t bmp5_set_osr_odr_press_config(const struct bmp5_osr_odr_press_config *osr_odr_press_cfg, struct bmp5_dev *dev)
{
    return BMP5_OK;
}

int8_t bmp5_set_iir_config(const struct bmp5_iir_config *iir_cfg, struct bmp5_dev *dev)
{
    return BMP5_OK;
}

/* Mocking Board support package */
struct bmp5_dev BMP581_Device;
