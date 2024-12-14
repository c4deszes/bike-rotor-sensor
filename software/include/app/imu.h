#if !defined(APP_IMU_H_)
#define APP_IMU_H_

#include <stdint.h>

typedef enum {
    IMU_Status_NotAvailable,
    IMU_Status_Ok,
    IMU_Status_Error
} IMU_Status_t;

void IMU_Initialize(void);

void IMU_Update(void);

IMU_Status_t IMU_GetStatus(void);

#endif // APP_IET_H_
