#include "app/comm.h"

#include "bsp/usart.h"

#include "line_protocol.h"
#include "line_api.h"

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
    // TODO: setup UART
    USART_Initialize(19200, &COMM_UsartBufferTx, &COMM_UsartBufferRx);
    USART_Enable();

    //LINE_Transport_Init(true);
    //LINE_App_Init();
    //LINE_Diag_Init(LINE_NODE_RotorSensor_DIAG_ADDRESS);
}

void COMM_UpdatePhy(void) {
    // TODO: uart read
    // pass to
    //LINE_Transport_Receive(x);
}

void LINE_Transport_WriteResponse(uint8_t size, uint8_t* payload, uint8_t checksum) {
    // TODO: uart write
}

void COMM_UpdateSignals(void) {
    // copy signal values to the frame objs

    // Global Speed state logic:
    // if spm global state is ok & iet state is ok -> ok
    // if spm global state is ok & iet state is fail -> slow response
    // if spm global state is unreliable -> unreliable
    // if spm global state is error and iet is fail -> error

    // Channel state logic
    // if channel off then -> off (phy)
    // if channel shorted -> shorted (phy)
    // if channel open -> open (phy)
}
