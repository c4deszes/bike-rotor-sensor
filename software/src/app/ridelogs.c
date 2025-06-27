#include "app/ridelogs.h"
#include "hal/dsu.h"
#include "hal/nvmctrl.h"
#include "app/ride.h"
#include "app/distance.h"

#include <stdint.h>
#include <stdbool.h>

static RIDELOGS_Container_t* RIDELOGS_Container1 = 0x3E000;
static RIDELOGS_Container_t* RIDELOGS_Container2 = 0x3E200;

static RIDELOGS_Container_t RIDELOGS_ContainerCache;
static RIDELOGS_Container_t* RIDELOGS_ContainerToWrite;
static RIDELOGS_Record_t RIDELOGS_ActiveRecord;
static bool RIDELOGS_RecordOverflow = false;

/**
 * @brief Checks if the given container is valid by calculating its CRC and comparing it with the stored CRC.
 * Note: only use this function for addresses in NVM memory
 * 
 * @param container 
 * @return true 
 * @return false 
 */
static bool RIDELOGS_IsContainerValid(RIDELOGS_Container_t* container) {
    uint32_t calculatedCrc = DSU_CalculateCRC32(DSU_CRC32_INITIAL, (uint8_t*)container, sizeof(RIDELOGS_Container_t) - sizeof(uint32_t));

    return (calculatedCrc == container->Crc);
}

static void RIDELOGS_LoadContainers(void)
{
    bool container1Valid = RIDELOGS_IsContainerValid(RIDELOGS_Container1);
    bool container2Valid = RIDELOGS_IsContainerValid(RIDELOGS_Container2);

    if (container1Valid && container2Valid) {
        // Both containers are valid, choose the one with the higher WriteCounter
        if (RIDELOGS_Container1->WriteCounter >= RIDELOGS_Container2->WriteCounter) {
            RIDELOGS_ContainerToWrite = RIDELOGS_Container2;
            RIDELOGS_ContainerCache = *RIDELOGS_Container1;
        }
        else {
            RIDELOGS_ContainerToWrite = RIDELOGS_Container1;
            RIDELOGS_ContainerCache = *RIDELOGS_Container2;
        }
    }
    else if (container1Valid) {
        // Only container 1 is valid
        RIDELOGS_ContainerToWrite = RIDELOGS_Container2;
        RIDELOGS_ContainerCache = *RIDELOGS_Container1;
    }
    else if (container2Valid) {
        // Only container 2 is valid
        RIDELOGS_ContainerToWrite = RIDELOGS_Container1;
        RIDELOGS_ContainerCache = *RIDELOGS_Container2;
    } else {
        // Neither container is valid, initialize a new one
        RIDELOGS_ContainerToWrite = RIDELOGS_Container1;
        RIDELOGS_ContainerCache.WriteCounter = 0;
        RIDELOGS_ContainerCache.KeyCounter = 0;
        for (int i = 0; i < 25; i++) {
            RIDELOGS_ContainerCache.Records[i].Identifier = 0;
            RIDELOGS_ContainerCache.Records[i].Duration = 0;
            RIDELOGS_ContainerCache.Records[i].Distance[0] = 0;
            RIDELOGS_ContainerCache.Records[i].Distance[1] = 0;
            RIDELOGS_ContainerCache.Records[i].Distance[2] = 0;
            RIDELOGS_ContainerCache.Records[i].Reserved = 0;
            RIDELOGS_ContainerCache.Records[i].CrankRevolutions = 0;
            RIDELOGS_ContainerCache.Records[i].TopSpeed = 0;
            RIDELOGS_ContainerCache.Records[i].AvgSpeed = 0;
            RIDELOGS_ContainerCache.Records[i].Elevation = 0;
            RIDELOGS_ContainerCache.Records[i].Descent = 0;
            RIDELOGS_ContainerCache.Records[i].Indications.flags = 0;
        }
        RIDELOGS_ContainerCache.Crc = 0; // Initialize CRC to 0
    }
    RIDELOGS_ContainerCache.WriteCounter += 1;
}

void RIDELOGS_Initialize(void) {
    RIDELOGS_LoadContainers();
    RIDELOGS_ResetActiveRecord(); // Reset the active record to default values
}

void RIDELOGS_UpdateActiveRecord(void)
{
    RIDELOGS_ActiveRecord.Duration = RIDE_GetDuration();

    uint32_t distance = DIST_GetDistance(); // Get distance from the distance module
    RIDELOGS_ActiveRecord.Distance[2] = (distance >> 16) & 0xFF; // High byte
    RIDELOGS_ActiveRecord.Distance[1] = (distance >> 8) & 0xFF; // Middle byte
    RIDELOGS_ActiveRecord.Distance[0] = distance & 0xFF; // Low byte

    RIDELOGS_ActiveRecord.Reserved = 0xFFu;
    RIDELOGS_ActiveRecord.CrankRevolutions = 0;                 // TODO: connect
    RIDELOGS_ActiveRecord.TopSpeed = RIDE_GetTopSpeed();
    RIDELOGS_ActiveRecord.AvgSpeed = RIDE_GetAverageSpeed();
    RIDELOGS_ActiveRecord.Elevation = 0;                        // TODO: connect
    RIDELOGS_ActiveRecord.Descent = 0;                          // TODO: connect

    // TODO: update flags based on ride status and sensor errors
    RIDELOGS_ActiveRecord.Indications.flags = 0; // Reset flags
}

void RIDELOGS_ResetActiveRecord(void) {
    RIDELOGS_ActiveRecord.Identifier = RIDELOGS_ContainerCache.KeyCounter + 1;
    RIDELOGS_ActiveRecord.Duration = 0;
    RIDELOGS_ActiveRecord.Distance[0] = 0;
    RIDELOGS_ActiveRecord.Distance[1] = 0;
    RIDELOGS_ActiveRecord.Distance[2] = 0;
    RIDELOGS_ActiveRecord.Reserved = 0;
    RIDELOGS_ActiveRecord.CrankRevolutions = 0;
    RIDELOGS_ActiveRecord.TopSpeed = 0;
    RIDELOGS_ActiveRecord.AvgSpeed = 0;
    RIDELOGS_ActiveRecord.Elevation = 0;
    RIDELOGS_ActiveRecord.Descent = 0;
    RIDELOGS_ActiveRecord.Indications.flags = 0; // Reset flags
}

void RIDELOGS_PushActiveRecord(void) {
    RIDELOGS_ContainerCache.Records[RIDELOGS_ContainerCache.LastRecordIndex] = RIDELOGS_ActiveRecord;
    RIDELOGS_ContainerCache.KeyCounter += 1; // Increment key counter for the new record
    RIDELOGS_ContainerCache.LastRecordIndex = RIDELOGS_ContainerCache.LastRecordIndex + 1;
    if (RIDELOGS_ContainerCache.LastRecordIndex >= 25) {
        RIDELOGS_RecordOverflow = true;
        RIDELOGS_ContainerCache.LastRecordIndex = 0; // Reset index if overflow occurs
    }
}

RIDELOGS_Record_t* RIDELOGS_GetRecord(uint16_t index) {
    return &RIDELOGS_ContainerCache.Records[index];
}

void RIDELOGS_SaveContainer(void)
{
    RIDELOGS_ContainerCache.Crc = DSU_SoftwareCRC32(DSU_CRC32_INITIAL, (uint8_t*)&RIDELOGS_ContainerCache, sizeof(RIDELOGS_Container_t) - sizeof(uint32_t));
    
    NVMCTRL_EraseRow((uint32_t)RIDELOGS_ContainerToWrite);
    NVMCTRL_EraseRow(((uint32_t)(RIDELOGS_ContainerToWrite)) + 4 * 64);

    NVMCTRL_WritePages((uint32_t)RIDELOGS_ContainerToWrite, (uint8_t*)&RIDELOGS_ContainerCache, sizeof(RIDELOGS_Container_t) / 64);

    RIDELOGS_ContainerCache.WriteCounter += 1;
    
    // Reset the active record
    RIDELOGS_ResetActiveRecord();
}
