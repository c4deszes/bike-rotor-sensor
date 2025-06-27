#if !defined(_APP_RIDELOGS_H)   
#define _APP_RIDELOGS_H

#include <stdint.h>
#include <assert.h>

typedef struct {
    uint16_t Identifier; // Timestamp of the ride log entry
    uint16_t Duration;  // Duration of the ride in seconds
    uint8_t Distance[3];  // Distance covered in meters
    uint8_t Reserved;
    
    uint16_t CrankRevolutions;

    uint16_t TopSpeed;  // Top speed in 0.1km/h
    uint16_t AvgSpeed; // Average speed in 0.1km/h

    uint16_t Elevation;
    uint16_t Descent;
    
    union {
        uint16_t flags; // Flags for various ride log states
        struct {
            // Configuration flags
            uint16_t LogOverflow : 1; // Log overflow flag
            uint16_t ConfigChanged : 1; // Configuration changed flag
            uint16_t QnhUnchanged : 1; // QNH unchanged flag
            
            uint16_t Reserved1 : 5;

            // Sensor error flags
            uint16_t FrontErr : 1;
            uint16_t RearErr : 1;
            uint16_t CadenceErr : 1;
            uint16_t ImuErr : 1;
            uint16_t PressSensErr : 1;
            uint16_t TempSensErr : 1;
            uint16_t DistanceErr : 1;
            uint16_t Reserved4 : 1;
        } bits;
    } Indications;
} RIDELOGS_Record_t;

typedef struct {
    uint16_t WriteCounter;
    uint16_t KeyCounter;
    uint8_t LastRecordIndex;
    uint8_t Reserved;
    RIDELOGS_Record_t Records[25];
    uint32_t Crc;
} RIDELOGS_Container_t;

static_assert(sizeof(RIDELOGS_Container_t) == 512, "RIDELOGS_Container_t must be 2 rows");

/**
 * @brief Initializes the ride logs container and sets default values.
 * 
 * The function checks the two containsers for validity and initializes the active container.
 */
void RIDELOGS_Initialize(void);

/**
 * @brief Updates the active ride log record with the current ride statistics.
 */
void RIDELOGS_UpdateActiveRecord(void);

/**
 * @brief Resets the active ride log record to default values.
 * 
 * This function sets the active record's identifier, duration, distance, and other fields to their initial state.
 */
void RIDELOGS_ResetActiveRecord(void);

void RIDELOGS_PushActiveRecord(void);

RIDELOGS_Record_t* RIDELOGS_GetRecord(uint16_t index);



void RIDELOGS_ResetContainer(void);

void RIDELOGS_SaveContainer(void);

#endif // _APP_RIDELOGS_H)  
