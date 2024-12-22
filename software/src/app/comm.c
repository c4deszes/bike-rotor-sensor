#include "app/comm.h"
#include "bsp/usart.h"

#include "line_protocol.h"
#include "line_api.h"
#include "flash_line_api.h"
#include "flash_line_diag.h"
#include "uds_api.h"
#include "bl/api.h"
#include "hal/dsu.h"
#include "app/imu.h"
#include "app/cadence.h"
#include "app/ride.h"

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
    .U_status = LINE_DIAG_POWER_STATUS_VOLTAGE_OK,          // TODO: implement voltage measurement
    .BOD_status = LINE_DIAG_POWER_STATUS_BOD_NONE,          // TODO: implement BOD
    .I_operating = LINE_DIAG_POWER_STATUS_OP_CURRENT(100),  // TODO: implement current measurement
    .I_sleep = LINE_DIAG_POWER_STATUS_SLEEP_CURRENT(100)    // TODO: implement current measurement
};

// TODO: use version from CMake
static LINE_Diag_SoftwareVersion_t sw_version = {
    .major = 0,
    .minor = 1,
    .patch = 0
};

// TODO: implement
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

static uint32_t COMM_EncodeDistance(uint32_t distance) {
    return distance / 1000; // from mm to m
}

static uint8_t COMM_EncoderDistanceStatus(DIST_Status_t status) {
    switch (status) {
        case DIST_Status_NotAvailable: return LINE_ENCODER_DistanceStatusEncoder_NotAvailable;
        case DIST_Status_Ok: return LINE_ENCODER_DistanceStatusEncoder_Ok;
        case DIST_Status_Error: return LINE_ENCODER_DistanceStatusEncoder_Error;
        case DIST_Status_PermanentError: return LINE_ENCODER_DistanceStatusEncoder_PermanentError;
    }
    return LINE_ENCODER_DistanceStatusEncoder_Error;
}

static uint8_t COMM_EncodeCadenceStatus(CADENCE_Status_t status) {
    switch (status) {
        case CADENCE_Status_NotAvailable: return LINE_ENCODER_CadenceStatusEncoder_NotAvailable;
        case CADENCE_Status_Ok: return LINE_ENCODER_CadenceStatusEncoder_Ok;
        case CADENCE_Status_Coasting: return LINE_ENCODER_CadenceStatusEncoder_Coasting;
        case CADENCE_Status_Error: return LINE_ENCODER_CadenceStatusEncoder_Error;
    }
    return LINE_ENCODER_CadenceStatusEncoder_Error;
}

static uint8_t COMM_EncodeRideStatus(RIDE_Status_t status) {
    switch (status) {
        case RIDE_Status_NotStarted: return LINE_ENCODER_RideStatusEncoder_NotStarted;
        case RIDE_Status_Active: return LINE_ENCODER_RideStatusEncoder_Active;
        case RIDE_Status_Idle: return LINE_ENCODER_RideStatusEncoder_Idle;
        case RIDE_Status_Paused: return LINE_ENCODER_RideStatusEncoder_Paused;
    }
    return LINE_ENCODER_RideStatusEncoder_Idle;
}

// TODO: split task up, e.g.: ride stats only need to be updated when every 1s
void COMM_UpdateSignals(void) {
    LINE_Request_SpeedStatus_data.fields.Speed = SPEED_GetSpeed();
    LINE_Request_SpeedStatus_data.fields.SpeedState = COMM_EncodeSpeedState(SPEED_GetStatus());
    // TODO: braking
    LINE_Request_SpeedStatus_data.fields.BrakeState = LINE_ENCODER_BrakeStateEncoder_Disabled;
    // TODO: lockup/slip
    LINE_Request_SpeedStatus_data.fields.FrontWheelSlip = LINE_ENCODER_WheelSlipEncoder_NotSlipping;
    LINE_Request_SpeedStatus_data.fields.FrontWheelLockup = LINE_ENCODER_WheelLockupEncoder_NoLockup;
    LINE_Request_SpeedStatus_data.fields.RearWheelSlip = LINE_ENCODER_WheelSlipEncoder_NotSlipping;
    LINE_Request_SpeedStatus_data.fields.RearWheelLockup = LINE_ENCODER_WheelLockupEncoder_NoLockup;

    LINE_Request_DrivetrainStatus_data.fields.Cadence = CAD_GetCadence();
    LINE_Request_DrivetrainStatus_data.fields.CadenceStatus = COMM_EncodeCadenceStatus(CAD_GetStatus());

    LINE_Request_RideStatus_data.fields.Duration = RIDE_GetDuration();
    LINE_Request_RideStatus_data.fields.RideStatus = COMM_EncodeRideStatus(RIDE_GetStatus());
    LINE_Request_RideStatus_data.fields.DistanceStatus = COMM_EncoderDistanceStatus(DIST_GetStatus());
    LINE_Request_RideStatus_data.fields.Distance = COMM_EncodeDistance(DIST_GetDistance());

    LINE_Request_RoadStatus_data.fields.Altitude = LINE_ENCODER_AltitudeEncoder_Encode(ALT_GetAltitude());
    // TODO: connect from ride.c
    LINE_Request_RoadStatus_data.fields.Grade = LINE_ENCODER_GradeEncoder_Encode(0);
    // TODO: connect from road quality component
    LINE_Request_RoadStatus_data.fields.RoadQuality = LINE_ENCODER_RoadQualityEncoder_NotMeasured;
    // TODO: connect from ITPMS component
    LINE_Request_RoadStatus_data.fields.ITPMS = LINE_ENCODER_ITPMSEncoder_Stopped;

    LINE_Request_RideStatistics_data.fields.TopSpeed = 0;
    LINE_Request_RideStatistics_data.fields.AverageSpeed = 0;
    LINE_Request_RideStatistics_data.fields.Elevation = 0;
    LINE_Request_RideStatistics_data.fields.Descent = 0;
}

void COMM_UpdateDebugSignals() {
    LINE_Request_RotorSensorMotionDebug_data.fields.aX = IMU_Accel.x;
    LINE_Request_RotorSensorMotionDebug_data.fields.aY = IMU_Accel.y;
    LINE_Request_RotorSensorMotionDebug_data.fields.aZ = IMU_Accel.z;

    LINE_Request_RotorSensorMotionDebug_data.fields.gX = IMU_Gyro.x;
    LINE_Request_RotorSensorMotionDebug_data.fields.gY = IMU_Gyro.y;
    LINE_Request_RotorSensorMotionDebug_data.fields.gZ = IMU_Gyro.z;

    LINE_Request_RotorSensorSpeedDebug_data.fields.FrontSpeed = SPEED_FrontWheel.speed;
    LINE_Request_RotorSensorSpeedDebug_data.fields.RearSpeed = SPEED_RearWheel.speed;
    LINE_Request_RotorSensorSpeedDebug_data.fields.FrontSensorStatus = COMM_EncodeSpeedSensorState(SEC_GetChannelState(SPM_FRONT_SENSOR_CHANNEL));
    LINE_Request_RotorSensorSpeedDebug_data.fields.RearSensorStatus = COMM_EncodeSpeedSensorState(SEC_GetChannelState(SPM_REAR_SENSOR_CHANNEL));
    LINE_Request_RotorSensorSpeedDebug_data.fields.CrankSensorStatus = COMM_EncodeSpeedSensorState(SEC_GetChannelState(SPM_CRANK_SENSOR_CHANNEL));
    LINE_Request_RotorSensorSpeedDebug_data.fields.CrankPosition = 0;

    LINE_Request_RotorSensorPressureDebug_data.fields.Pressure = ALT_GetPressure();
}
