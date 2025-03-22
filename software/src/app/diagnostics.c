#include "app/diagnostics.h"

#include "line_protocol.h"
#include "line_api.h"
#include "flash_line_api.h"
#include "flash_line_diag.h"
#include "metainfo.h"
#include "app/current.h"
#include "app/speed.h"
#include "app/altitude.h"
#include "hal/dsu.h"
#include "app/cadence.h"

/* Standard diagnostic variables */
static LINE_Diag_SoftwareVersion_t DIAG_SwVersion = {
    .major = APP_SW_MAJOR,
    .minor = APP_SW_MINOR,
    .patch = APP_SW_PATCH
};
static LINE_Diag_PowerStatus_t DIAG_PowerStatus = {
    .U_measured = 0,
    .I_operating = 0,
    .I_sleep = LINE_DIAG_POWER_STATUS_SLEEP_CURRENT(100)
};
static uint8_t DIAG_OperationStatus;

static bool DIAG_BootResetFlag;

void DIAG_Initialize(void) {
    DIAG_BootResetFlag = false;
    DIAG_OperationStatus = LINE_DIAG_OP_STATUS_INIT;
}

void DIAG_Update(void) {
    /* Update the operation status */
    if (SPEED_GetStatus() == speed_status_error) {
        /* Loss of primary function */
        DIAG_OperationStatus = LINE_DIAG_OP_STATUS_ERROR;
    }
    else if (
        CAD_GetStatus() == CADENCE_Status_Error ||
        ALT_GetStatus() == ALT_Status_Error ||
        ALT_GetStatus() == ALT_Status_PermanentError ||
        SPEED_FrontWheel.state != speed_status_ok ||
        SPEED_RearWheel.state != speed_status_ok
    ) {
        /* Loss of secondary functions */
        DIAG_OperationStatus = LINE_DIAG_OP_STATUS_WARN;
    }
    else {
        DIAG_OperationStatus = LINE_DIAG_OP_STATUS_OK;
    }

    /* Update the power status */
    // TODO: update voltage
    DIAG_PowerStatus.I_operating = CURRENT_GetCurrent();
}

bool DIAG_BootRequest(void) {
    return DIAG_BootResetFlag;
}

LINE_Diag_PowerStatus_t* LINE_Diag_BicycleNetwork_RotorSensor_GetPowerStatus(void) {
    return &DIAG_PowerStatus;
}

LINE_Diag_SoftwareVersion_t* LINE_Diag_BicycleNetwork_RotorSensor_GetSoftwareVersion(void) {
    return &DIAG_SwVersion;
}

uint8_t LINE_Diag_BicycleNetwork_RotorSensor_GetOperationStatus(void) {
    return DIAG_OperationStatus;
}

uint32_t LINE_Diag_BicycleNetwork_RotorSensor_GetSerialNumber(void) {
    return DSU_GetSerialNumber32();
}



void LINE_Diag_BicycleNetwork_RotorSensor_OnWakeup(void) {

}
void LINE_Diag_BicycleNetwork_RotorSensor_OnIdle(void) {
    // TODO: implement
}
void LINE_Diag_BicycleNetwork_RotorSensor_OnShutdown(void) {
    // TODO: implement
}
void LINE_Diag_BicycleNetwork_RotorSensor_OnConditionalChangeAddress(uint8_t old_address, uint8_t new_address) {
    // TODO: implement
}

fl_BootEntryResponse_t FLASH_BL_EnterBoot(void) {
    fl_BootEntryResponse_t response;
    // TODO: only allow boot entry when idle
    DIAG_BootResetFlag = true;

    response.entry_status = FLASH_LINE_BOOT_ENTRY_SUCCESS;
    response.serial_number = LINE_Diag_BicycleNetwork_RotorSensor_GetSerialNumber();

    return response;
}

