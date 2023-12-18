#include "app/comm.h"
#include "bsp/usart.h"

#include "line_protocol.h"
#include "line_api.h"

#include "app/spm.h"
#include "app/sec.h"

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
    return 0xABCDEF01;
}

LINE_Diag_SoftwareVersion_t* LINE_Diag_GetSoftwareVersion(void) {
    return &sw_version;
}

void COMM_Initialize(void) {
    USART_Initialize(19200, &COMM_UsartBufferTx, &COMM_UsartBufferRx);
    USART_Enable();

    LINE_Transport_Init(true);
    LINE_App_Init();
    LINE_Diag_Init(LINE_NODE_RotorSensor_DIAG_ADDRESS);
}

void COMM_UpdatePhy(void) {
    uint8_t length = USART_Available();
    while (length > 0) {
        uint8_t data = USART_Read();
        LINE_Transport_Receive(data);
        length--;
    }
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

uint8_t COMM_EncodeGlobalSpeedState(spm_speed_state_t state) {
    switch (state) {
        case spm_speed_state_ok: return LINE_ENCODER_GlobalSpeedStateEncoder_Ok;
        case spm_speed_slow_response: return LINE_ENCODER_GlobalSpeedStateEncoder_SlowResponse;
        case spm_speed_state_unreliable: return LINE_ENCODER_GlobalSpeedStateEncoder_Unreliable;
        case spm_speed_state_error: return LINE_ENCODER_GlobalSpeedStateEncoder_Error;
    }
    return LINE_ENCODER_GlobalSpeedStateEncoder_Error;
}

uint8_t COMM_EncodeChannelSpeedState(sec_state_t sensor, spm_speed_t speed) {
    if (sensor == sec_state_off) {
        return LINE_ENCODER_ChannelSpeedStateEncoder_Off;
    }
    else if (sensor == sec_state_short) {
        return LINE_ENCODER_ChannelSpeedStateEncoder_OutputShorted;
    }
    else if (sensor == sec_state_open) {
        return LINE_ENCODER_ChannelSpeedStateEncoder_OutputOpen;
    }
    else if (sensor == sec_state_error) {
        return LINE_ENCODER_ChannelSpeedStateEncoder_SensorError;
    }
    else if (speed == spm_speed_state_unreliable) {
        return LINE_ENCODER_ChannelSpeedStateEncoder_SpeedUnreliable;
    }
    else if (speed == sec_state_warning) {
        return LINE_ENCODER_ChannelSpeedStateEncoder_SensorWarning;
    }
    else if (speed == spm_speed_state_ok) {
        return LINE_ENCODER_ChannelSpeedStateEncoder_Ok;
    }

    return LINE_ENCODER_ChannelSpeedStateEncoder_OutputShorted;
}

void COMM_UpdateSignals(void) {
    LINE_Request_SpeedStatus_data.fields.Speed = spm_global_speed;
    LINE_Request_SpeedStatus_data.fields.SpeedState = COMM_EncodeGlobalSpeedState(spm_global_state);

    // TODO: Retrieve SEC state using proper channel numbers
    LINE_Request_SpeedStatus_data.fields.FrontSpeedState = COMM_EncodeChannelSpeedState(SEC_GetChannelState(0), spm_front_wheel.state);
    LINE_Request_SpeedStatus_data.fields.RearSpeedState = COMM_EncodeChannelSpeedState(SEC_GetChannelState(1), spm_rear_wheel.state);
}
