#if !defined(APP_IMU_H_)
#define APP_IMU_H_

#include <stdint.h>

typedef enum {
    IMU_Status_NotAvailable,
    IMU_Status_Ok,
    IMU_Status_Error,
    IMU_Status_PermanentError
} IMU_Status_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} IMU_Vector_t;

extern IMU_Vector_t IMU_Accel;
extern IMU_Vector_t IMU_Gyro;

/**
 * @brief Initializes the IMU service
 */
void IMU_Initialize(void);

/**
 * @brief Updates the IMU state and data
 */
void IMU_Update(void);

/**
 * @brief Returns the current IMU status
 */
IMU_Status_t IMU_GetStatus(void);

#endif // APP_IET_H_
