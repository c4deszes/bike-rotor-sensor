# Board support configuration
set(MCU_NAME "attiny416")

# Hardware Abstraction configuration
add_library(tiny-hal-cfg INTERFACE)
target_compile_options(tiny-hal-cfg INTERFACE -mmcu=${MCU_NAME})
target_link_options(tiny-hal-cfg INTERFACE -mmcu=${MCU_NAME})

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
target_include_directories(rotor-sensor-bsp PUBLIC api)
target_include_directories(rotor-sensor-bsp PRIVATE include)
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

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    enable_lto(rotor-sensor-bsp)
endif()
