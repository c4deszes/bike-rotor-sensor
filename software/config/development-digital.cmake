#set(BOARD_TYPE "STANDARD")
set(BOARD_TYPE "XPLAINED")

set(BOARD_SENSOR_LOC "EXTERNAL")
set(BOARD_SENSOR_PHY "OPTICAL")

#set(BOARD_SENSOR_TYPE "ANALOG")
set(BOARD_SENSOR_TYPE "DIGITAL")

set(BOARD_SENSOR_NOISE_FILTER "ENABLED")
#set(BOARD_SENSOR_NOISE_FILTER "DISABLED")

set(BOARD_SENSOR_ANALOG_REF "UNSET")
#set(BOARD_SENSOR_ANALOG_REF "EXTERNAL")
#set(BOARD_SENSOR_ANALOG_REF "0V55")
#set(BOARD_SENSOR_ANALOG_REF "1V1")
#set(BOARD_SENSOR_ANALOG_REF "1V5")
#set(BOARD_SENSOR_ANALOG_REF "2V5")
#set(BOARD_SENSOR_ANALOG_REF "4V3")

option(APP_BOOTLOADER_ENABLED "Enables bootloader for the application" ON)
