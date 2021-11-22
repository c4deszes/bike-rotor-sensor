# Board support configuration
if (BOARD_TYPE STREQUAL "XPLAINED")
    set(MCU_NAME "attiny416")
elseif(BOARD_TYPE STREQUAL "STANDARD")
    set(MCU_NAME "attiny202")
else()
    message(SEND_ERROR "Unknown board type '${BOARD_TYPE}'!")
endif()

# Hardware Abstraction configuration
add_library(tiny-hal-cfg INTERFACE)
target_compile_options(tiny-hal-cfg INTERFACE -mmcu=${MCU_NAME})
target_link_options(tiny-hal-cfg INTERFACE -mmcu=${MCU_NAME})

# Board support package public interface
add_library(rotor-sensor-bsp-api INTERFACE)
target_include_directories(rotor-sensor-bsp-api INTERFACE api)
target_compile_definitions(
    rotor-sensor-bsp-api
    INTERFACE
    "BOARD_TYPE_${BOARD_TYPE}"
    "BOARD_SENSOR_LOC_${BOARD_SENSOR_LOC}"
    "BOARD_SENSOR_PHY_${BOARD_SENSOR_PHY}"
    "BOARD_SENSOR_TYPE_${BOARD_SENSOR_TYPE}"
    "BOARD_SENSOR_ANALOG_REF_${BOARD_SENSOR_ANALOG_REF}"
)

# Board support package private interface
add_library(rotor-sensor-bsp-private-api INTERFACE)
target_include_directories(rotor-sensor-bsp-private-api INTERFACE include)

set(SOURCES
    src/board.c
    src/osh.c
    src/ish.c
)
if(${BOARD_SENSOR_TYPE} STREQUAL "ANALOG")
    set(SOURCES ${SOURCES} src/ish_analog.c)
elseif(${BOARD_SENSOR_TYPE} STREQUAL "DIGITAL")
    set(SOURCES ${SOURCES} src/ish_digital.c)
else()
    message(SEND_ERROR "Unknown sensor type: ${BOARD_SENSOR_TYPE}!")
endif()

# Board support library
add_library(
    rotor-sensor-bsp STATIC EXCLUDE_FROM_ALL
    ${SOURCES}
)
target_link_libraries(rotor-sensor-bsp PUBLIC rotor-sensor-bsp-api tiny-hal)
target_link_libraries(rotor-sensor-bsp PRIVATE rotor-sensor-bsp-private-api)
target_compile_definitions(
    rotor-sensor-bsp
    PRIVATE
    "BOARD_TYPE_${BOARD_TYPE}"
    "BOARD_SENSOR_LOC_${BOARD_SENSOR_LOC}"
    "BOARD_SENSOR_PHY_${BOARD_SENSOR_PHY}"
    "BOARD_SENSOR_TYPE_${BOARD_SENSOR_TYPE}"
    "BOARD_SENSOR_ANALOG_REF_${BOARD_SENSOR_ANALOG_REF}"
)
target_link_libraries(rotor-sensor-bsp PRIVATE tiny-hal)

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    enable_lto(rotor-sensor-bsp)
endif()

add_library(
    rotor-sensor-bsp-test STATIC EXCLUDE_FROM_ALL
    ${SOURCES}
)
target_link_libraries(rotor-sensor-bsp-test PUBLIC rotor-sensor-bsp-api tiny-hal-fake)
target_link_libraries(rotor-sensor-bsp-test PUBLIC rotor-sensor-bsp-private-api)
