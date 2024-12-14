#include "app/imu.h"
#include "app/config.h"

#include "bsp/bmi088.h"
#include "common/swtimer.h"

#include "bmi08.h"
#include "bmi08_defs.h"
#include "bmi08x.h"

static IMU_Status_t IMU_Status = IMU_Status_NotAvailable;
// TODO: make these public somehow
struct bmi08_sensor_data bmi08_accel;
struct bmi08_sensor_data bmi08_gyro;

static swtimer_t* IMU_SensorTimer = NULL;
static bool IMU_SensorInitialized = false;
static int8_t IMU_BMI088_InitCode = BMI08_OK;

static void BMI088_SetupSensor() {
    IMU_BMI088_InitCode = bmi08xa_init(&BMI088_Device);

    if (IMU_BMI088_InitCode != BMI08_OK) {
        return;
    }

    IMU_BMI088_InitCode = bmi08g_init(&BMI088_Device);

    if (IMU_BMI088_InitCode != BMI08_OK) {
        return;
    }

    IMU_BMI088_InitCode = bmi08a_load_config_file(&BMI088_Device);

    if (IMU_BMI088_InitCode != BMI08_OK) {
        return;
    }

    BMI088_Device.accel_cfg.odr = BMI08_ACCEL_ODR_100_HZ;
    BMI088_Device.accel_cfg.range = BMI088_ACCEL_RANGE_6G;
    BMI088_Device.accel_cfg.power = BMI08_ACCEL_PM_ACTIVE; /*user_accel_power_modes[user_bmi088_accel_low_power]; */
    BMI088_Device.accel_cfg.bw = BMI08_ACCEL_BW_NORMAL; /* Bandwidth and OSR are same */

    IMU_BMI088_InitCode = bmi08a_set_power_mode(&BMI088_Device);
    if (IMU_BMI088_InitCode != BMI08_OK) {
        return;
    }

    IMU_BMI088_InitCode = bmi08xa_set_meas_conf(&BMI088_Device);
    if (IMU_BMI088_InitCode != BMI08_OK) {
        return;
    }

    BMI088_Device.gyro_cfg.odr = BMI08_GYRO_BW_12_ODR_100_HZ;
    BMI088_Device.gyro_cfg.range = BMI08_GYRO_RANGE_125_DPS;
    BMI088_Device.gyro_cfg.bw = BMI08_GYRO_BW_12_ODR_100_HZ;
    BMI088_Device.gyro_cfg.power = BMI08_GYRO_PM_NORMAL;

    IMU_BMI088_InitCode = bmi08g_set_power_mode(&BMI088_Device);
    if (IMU_BMI088_InitCode != BMI08_OK) {
        return;
    }

    IMU_BMI088_InitCode = bmi08g_set_meas_conf(&BMI088_Device);
    if (IMU_BMI088_InitCode != BMI08_OK) {
        return;
    }
}

void IMU_Initialize(void) {
    IMU_Status = IMU_Status_NotAvailable;
    IMU_SensorTimer = SWTIMER_Create();
    SWTIMER_Setup(IMU_SensorTimer, IMU_SENSOR_STARTUP_TIME);
}

void IMU_Update(void) {
#if IMU_SENSOR_ENABLED == 1
    // TODO: implement startup attempts and delay
    if (!IMU_SensorInitialized && SWTIMER_Elapsed(IMU_SensorTimer)) {
        BMI088_SetupSensor();

        if (IMU_BMI088_InitCode == BMI08_OK) {
            IMU_Status = IMU_Status_Ok;
        }
        else {
            IMU_Status = IMU_Status_Error;
        }

        IMU_SensorInitialized = true;
    }

    // TODO: implement polling interval
    if (IMU_SensorInitialized && IMU_BMI088_InitCode == BMI08_OK) {
        int8_t bmi08_read_status;
#if IMU_SENSOR_ACCEL_ENABLED == 1
        bmi08_read_status = bmi08a_get_data(&bmi08_accel, &BMI088_Device);

        // TODO: error handling
#endif

#if IMU_SENSOR_GYRO_ENABLED == 1
        bmi08_read_status = bmi08g_get_data(&bmi08_gyro, &BMI088_Device);

        // TODO: error handling
#endif
    }
#endif
}

IMU_Status_t IMU_GetStatus(void) {
    return IMU_Status;
}
