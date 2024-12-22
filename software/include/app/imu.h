#if !defined(APP_IMU_H_)
#define APP_IMU_H_

#include <stdint.h>

typedef enum {
    IMU_Status_NotAvailable,
    IMU_Status_Ok,
    IMU_Status_Error
} IMU_Status_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} IMU_Vector_t;

extern IMU_Vector_t IMU_Accel;
extern IMU_Vector_t IMU_Gyro;

void IMU_Initialize(void);

void IMU_Update(void);

IMU_Status_t IMU_GetStatus(void);

#endif // APP_IET_H_
