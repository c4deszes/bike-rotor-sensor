#include "app/config.h"
#include "uds_gen.h"

CONFIG_Properties_t CONFIG_Props;

void CONFIG_LoadFlashProperties(void) {
    // Load properties from flash memory
    // ...
}

void CONFIG_ReloadUdsProperties(void) {
    // Reload properties from UDS
    // ...
    CONFIG_Props.FrontWheel_Circumference = UDS_Properties_RotorSensor.FrontWheel_Circumference;
    CONFIG_Props.RearWheel_Circumference = UDS_Properties_RotorSensor.RearWheel_Circumference;
    CONFIG_Props.CrankArm_Circumference = UDS_Properties_RotorSensor.CrankArm_Circumference;

    CONFIG_Props.FrontWheel_PoleCount = UDS_Properties_RotorSensor.FrontWheel_PoleCount;
    CONFIG_Props.RearWheel_PoleCount = UDS_Properties_RotorSensor.RearWheel_PoleCount;
    CONFIG_Props.CrankArm_PoleCount = UDS_Properties_RotorSensor.CrankArm_PoleCount;

    // TODO: sensor types

    CONFIG_Props.Gear_HoldTime = UDS_Properties_RotorSensor.Gear_HoldTime;
    CONFIG_Props.Gear_MaxGears = UDS_Properties_RotorSensor.Gear_MaxGears;
    CONFIG_Props.Gear_ChainringTeeth = UDS_Properties_RotorSensor.Gear_ChainringTeeth;
    CONFIG_Props.Gear_CassetteTeeth[0] = UDS_Properties_RotorSensor.Gear_CasetteTeeth1;
    CONFIG_Props.Gear_CassetteTeeth[1] = UDS_Properties_RotorSensor.Gear_CasetteTeeth2;
    CONFIG_Props.Gear_CassetteTeeth[2] = UDS_Properties_RotorSensor.Gear_CasetteTeeth3;
    CONFIG_Props.Gear_CassetteTeeth[3] = UDS_Properties_RotorSensor.Gear_CasetteTeeth4;
    CONFIG_Props.Gear_CassetteTeeth[4] = UDS_Properties_RotorSensor.Gear_CasetteTeeth5;
    CONFIG_Props.Gear_CassetteTeeth[5] = UDS_Properties_RotorSensor.Gear_CasetteTeeth6;
    CONFIG_Props.Gear_CassetteTeeth[6] = UDS_Properties_RotorSensor.Gear_CasetteTeeth7;
    CONFIG_Props.Gear_CassetteTeeth[7] = UDS_Properties_RotorSensor.Gear_CasetteTeeth8;
    CONFIG_Props.Gear_CassetteTeeth[8] = UDS_Properties_RotorSensor.Gear_CasetteTeeth9;
    CONFIG_Props.Gear_CassetteTeeth[9] = UDS_Properties_RotorSensor.Gear_CasetteTeeth10;
    CONFIG_Props.Gear_CassetteTeeth[10] = UDS_Properties_RotorSensor.Gear_CasetteTeeth11;
    CONFIG_Props.Gear_CassetteTeeth[11] = UDS_Properties_RotorSensor.Gear_CasetteTeeth12;

    CONFIG_Props.Ride_AutoStart = UDS_Properties_RotorSensor.Ride_AutoStart;
    CONFIG_Props.Ride_AutoPause = UDS_Properties_RotorSensor.Ride_AutoPause;
    CONFIG_Props.Ride_AutoResume = UDS_Properties_RotorSensor.Ride_AutoResume;
    CONFIG_Props.Ride_IdleSpeed = UDS_Properties_RotorSensor.Ride_IdleSpeed;
    CONFIG_Props.Ride_IdleTimeout = UDS_Properties_RotorSensor.Ride_IdleTimeout;
    CONFIG_Props.Ride_PauseTimeout = UDS_Properties_RotorSensor.Ride_PauseTimeout;
    CONFIG_Props.Ride_ResumeTimeout = UDS_Properties_RotorSensor.Ride_ResumeTimeout;

    CONFIG_Props.Altitude_Enabled = UDS_Properties_RotorSensor.Altitude_Enabled;
    //CONFIG_Props.Altitude_QNH = UDS_Properties_RotorSensor.Altitude_QNH;

    //CONFIG_Props.ITPMS_AutoStart = UDS_Properties_RotorSensor.ITPMS_AutoStart;
    //CONFIG_Props.ITPMS_MinimumSpeed = UDS_Properties_RotorSensor.ITPMS_MinimumSpeed;
    //CONFIG_Props.ITPMS_SpeedThreshold = UDS_Properties_RotorSensor.ITPMS_SpeedThreshold;
}
