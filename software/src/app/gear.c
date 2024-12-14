#include "app/gear.h"

#include "app/config.h"

#include "app/cadence.h"
#include "app/speed.h"

static uint8_t GEAR_CurrentGear = 0;

static uint8_t GEAR_CassetteRange = 0;
static uint8_t GEAR_CassetteGears[GEAR_MAX_GEARS] = {0};

void GEAR_Initialize(void) {
    GEAR_CurrentGear = 0;
    GEAR_CassetteRange = GEAR_CASSETTE_DEFAULT_RANGE;
    GEAR_CassetteGears[0] = GEAR_CASSETTE_DEFAULT_GEAR_1;
    GEAR_CassetteGears[1] = GEAR_CASSETTE_DEFAULT_GEAR_2;
    GEAR_CassetteGears[2] = GEAR_CASSETTE_DEFAULT_GEAR_3;
    GEAR_CassetteGears[3] = GEAR_CASSETTE_DEFAULT_GEAR_4;
    GEAR_CassetteGears[4] = GEAR_CASSETTE_DEFAULT_GEAR_5;
    GEAR_CassetteGears[5] = GEAR_CASSETTE_DEFAULT_GEAR_6;
    GEAR_CassetteGears[6] = GEAR_CASSETTE_DEFAULT_GEAR_7;
    GEAR_CassetteGears[7] = GEAR_CASSETTE_DEFAULT_GEAR_8;
    GEAR_CassetteGears[8] = GEAR_CASSETTE_DEFAULT_GEAR_9;
    GEAR_CassetteGears[9] = GEAR_CASSETTE_DEFAULT_GEAR_10;
    GEAR_CassetteGears[10] = GEAR_CASSETTE_DEFAULT_GEAR_11;
    GEAR_CassetteGears[11] = GEAR_CASSETTE_DEFAULT_GEAR_12;
}

void GEAR_Update(void) {
    
}

uint8_t GEAR_GetGear(void) {
    return GEAR_CurrentGear;
}
