#include "app/comm.h"
#include "bsp/usart.h"

#include "line_protocol.h"
#include "line_api.h"
#include "flash_line_api.h"
#include "flash_line_diag.h"
#include "uds_api.h"
#include "bl/api.h"
#include "hal/dsu.h"

#include "app/speed.h"
#include "app/sec.h"
#include "app/altitude.h"
#include "app/distance.h"
#include "app/config.h"

#include "bmi08.h"
#include "bmi08_defs.h"
#include "bmi08x.h"

RINGBUFFER_8(COMM_UsartBufferTx, 128);
RINGBUFFER_8(COMM_UsartBufferRx, 128);

static LINE_Diag_PowerStatus_t power_status = {
    .U_status = LINE_DIAG_POWER_STATUS_VOLTAGE_OK,
    .BOD_status = LINE_DIAG_POWER_STATUS_BOD_NONE,
    .I_operating = LINE_DIAG_POWER_STATUS_OP_CURRENT(100),
    .I_sleep = LINE_DIAG_POWER_STATUS_SLEEP_CURRENT(100)
};
static LINE_Diag_SoftwareVersion_t sw_version = {
    .major = 0,
    .minor = 1,
    .patch = 0
};

uint8_t LINE_Diag_GetOperationStatus(void) {
    return LINE_DIAG_OP_STATUS_OK;
}

LINE_Diag_PowerStatus_t* LINE_Diag_GetPowerStatus(void) {
    return &power_status;
}

uint32_t LINE_Diag_GetSerialNumber(void) {
    return DSU_GetSerialNumber32();
}

LINE_Diag_SoftwareVersion_t* LINE_Diag_GetSoftwareVersion(void) {
    return &sw_version;
}

void COMM_Initialize(void) {
    USART_Initialize(LINE_NETWORK_BicycleNetwork1_BAUDRATE, &COMM_UsartBufferTx, &COMM_UsartBufferRx);
    USART_Enable();

    LINE_Transport_Init(true);
    LINE_App_Init();
    LINE_Diag_SetAddress(LINE_NODE_RotorSensor_DIAG_ADDRESS);

    FLASH_LINE_Init(FLASH_LINE_APPLICATION_MODE);

    UDS_Init();
    UDS_LINE_Init();
}

void COMM_UpdatePhy(void) {
    uint8_t length = USART_Available();
    while (length > 0) {
        uint8_t data = USART_Read();
        LINE_Transport_Receive(data);
        length--;
    }

    LINE_Transport_Update(1);
}

void LINE_Transport_WriteResponse(uint8_t size, uint8_t* payload, uint8_t checksum) {
    const uint8_t fix = 69;
    USART_WriteData(&size, sizeof(uint8_t));
    // TODO: fix for skipped 3rd byte
    USART_WriteData(payload, 1);
    USART_WriteData(&fix, 1);
    USART_WriteData(payload+1, size-1);
    USART_WriteData(&checksum, sizeof(uint8_t));
    USART_FlushOutput();
}

void LINE_Transport_WriteRequest(uint16_t request) {
    return;
}

static bool bootResetFlag = false;

bool COMM_ResetRequest(void) {
    return false;
}

bool COMM_BootRequest(void) {
    return bootResetFlag;
}

uint8_t FLASH_BL_EnterBoot(void) {
    // TODO: only allow boot entry when idle

    bootResetFlag = true;

    return FLASH_LINE_BOOT_ENTRY_SUCCESS;
}

static uint8_t COMM_EncodeSpeedState(speed_status_t state) {
    switch (state) {
        case speed_status_ok: return LINE_ENCODER_SpeedStateEncoder_Ok;
        case speed_status_slow_response: return LINE_ENCODER_SpeedStateEncoder_SlowResponse;
        case speed_status_unreliable: return LINE_ENCODER_SpeedStateEncoder_Unreliable;
        case speed_status_error: return LINE_ENCODER_SpeedStateEncoder_Error;
    }
    return LINE_ENCODER_SpeedStateEncoder_Error;
}

static uint8_t COMM_EncodeSpeedSensorState(sec_state_t state) {
    switch (state) {
        case sec_state_ok: return LINE_ENCODER_SpeedSensorStateEncoder_Ok;
        case sec_state_warning: return LINE_ENCODER_SpeedSensorStateEncoder_SensorWarning;
        case sec_state_error: return LINE_ENCODER_SpeedSensorStateEncoder_SensorError;
        case sec_state_open: return LINE_ENCODER_SpeedSensorStateEncoder_OutputOpen;
        case sec_state_short: return LINE_ENCODER_SpeedSensorStateEncoder_OutputShorted;
        case sec_state_off: return LINE_ENCODER_SpeedSensorStateEncoder_Off;
    }
    return LINE_ENCODER_SpeedSensorStateEncoder_SensorError;
}

void COMM_UpdateSignals(void) {
    LINE_Request_SpeedStatus_data.fields.Speed = SPEED_GetSpeed();
    LINE_Request_SpeedStatus_data.fields.SpeedState = COMM_EncodeSpeedState(SPEED_GetStatus());

    LINE_Request_RideStatus_data.fields.Distance = DIST_GetDistance();
    LINE_Request_RideStatus_data.fields.MaxSpeed = 0;
    LINE_Request_RideStatus_data.fields.AverageSpeed = 0;
    LINE_Request_RideStatus_data.fields.Elevation = 0;
    LINE_Request_RideStatus_data.fields.Descent = 0;

    LINE_Request_RoadStatus_data.fields.Altitude = LINE_ENCODER_AltitudeEncoder_Encode(ALT_GetAltitude());
    LINE_Request_RoadStatus_data.fields.Gradient = 0;
    LINE_Request_RoadStatus_data.fields.RoadQuality = LINE_ENCODER_RoadQualityEncoder_NotMeasured;
}

void COMM_UpdateDebugSignals() {
    // LINE_Request_RotorSensorMotionDebug_data.fields.aX = bmi08_accel.x;
    // LINE_Request_RotorSensorMotionDebug_data.fields.aY = bmi08_accel.y;
    // LINE_Request_RotorSensorMotionDebug_data.fields.aZ = bmi08_accel.z;

    // LINE_Request_RotorSensorMotionDebug_data.fields.gX = bmi08_gyro.x;
    // LINE_Request_RotorSensorMotionDebug_data.fields.gY = bmi08_gyro.y;
    // LINE_Request_RotorSensorMotionDebug_data.fields.gZ = bmi08_gyro.z;

    LINE_Request_RotorSensorSpeedDebug_data.fields.FrontSpeed = SPEED_FrontWheel.speed;
    LINE_Request_RotorSensorSpeedDebug_data.fields.RearSpeed = SPEED_RearWheel.speed;
    LINE_Request_RotorSensorSpeedDebug_data.fields.FrontSensorStatus = COMM_EncodeSpeedSensorState(SEC_GetChannelState(SPM_FRONT_SENSOR_CHANNEL));
    LINE_Request_RotorSensorSpeedDebug_data.fields.RearSensorStatus = COMM_EncodeSpeedSensorState(SEC_GetChannelState(SPM_REAR_SENSOR_CHANNEL));

    LINE_Request_RotorSensorPeriodDebug_data.fields.Per = SPEED_FrontWheel.last_period;
    LINE_Request_RotorSensorPeriodDebug_data.fields.Pos = SPEED_RearWheel.last_positive;

    LINE_Request_RotorSensorSpeedDebug_data.fields.CrankSensorStatus = COMM_EncodeSpeedSensorState(SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL));
    LINE_Request_RotorSensorSpeedDebug_data.fields.CrankPosition = 0;

    LINE_Request_RotorSensorPressureDebug_data.fields.Pressure = ALT_GetPressure();
}
