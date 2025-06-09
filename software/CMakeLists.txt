cmake_minimum_required(VERSION 3.20)

project(
    rotor-sensor
    DESCRIPTION "Rotor Sensor"
    LANGUAGES C CXX ASM
    VERSION "0.5.0")

####################################################################################################
# Options
####################################################################################################
option(BUILD_UNIT_TESTS "Build unit tests" OFF)
option(BUILD_SIL_TESTS "Build SIL target and tests" OFF)

find_package(Python REQUIRED)

include(tools/cmake/CPM.cmake)

####################################################################################################
# Dependencies
####################################################################################################
CPMAddPackage("gh:c4deszes/samd21-hal#feature/new-peripherals2")
CPMAddPackage("gh:c4deszes/bike-line-protocol@0.2.0")
CPMAddPackage("gh:c4deszes/bike-flash-tool#feature/line-lib-update")
CPMAddPackage("gh:c4deszes/samd21-line-bootloader@0.3.0")
CPMAddPackage("gh:c4deszes/bike-uds-tool@0.2.0")

# Test configuration
enable_testing()
include(CTest)
set(CTEST_OUTPUT_ON_FAILURE ON)

include(CTest)
include(GoogleTest)
include(FetchContent)

####################################################################################################
# Documentation
####################################################################################################
add_subdirectory(docs)

####################################################################################################
# Code generation
####################################################################################################

# LINE protocol code generation
line_codegen(
    TARGET protocol-stack-api
    CONFIG tools/line/line_codegen.json
    ADAPTER
)

# LINE UDS extension code generation
uds_codegen(
    TARGET uds-lib
    CONFIG tools/line/uds_codegen.json
)

# Software version header generation
set(APP_GENCODE_DIR ${CMAKE_CURRENT_BINARY_DIR}/gencode)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/app/metainfo.h.in
               ${APP_GENCODE_DIR}/metainfo.h)

####################################################################################################
# Libraries
####################################################################################################

# SAMD21 HAL library
samx21_hal(samd21e18a ATSAMD21E18A)

# BMP581 Pressure sensor library
add_library(bmp5x-api INTERFACE)
target_compile_definitions(bmp5x-api INTERFACE
    BMP5_USE_FIXED_POINT
    BMP5_FIXED_POINT_DIGIT_PRECISION=2)
target_include_directories(bmp5x-api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/bmp5x)

add_library(bmp5x INTERFACE)
target_sources(bmp5x INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/bmp5x/bmp5.c
)
target_link_libraries(bmp5x INTERFACE bmp5x-api)

# BMI088 IMU library
add_library(bmi08x-api INTERFACE)
target_include_directories(bmi08x-api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib/bmi08x)

add_library(bmi08x INTERFACE)
target_sources(bmi08x INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/bmi08x/bmi08a.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/bmi08x/bmi08g.c
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/bmi08x/bmi08xa.c
)
target_link_libraries(bmi08x INTERFACE bmi08x-api)

####################################################################################################
# Application
####################################################################################################
add_executable(
    application
    EXCLUDE_FROM_ALL

    # Application layer
    src/app/altitude.c
    src/app/cadence.c
    src/app/comm.c
    src/app/config.c
    src/app/current.c
    src/app/diagnostics.c
    src/app/distance.c
    src/app/gear.c
    src/app/imu.c
    src/app/init.c
    src/app/itpms.c
    src/app/main.c
    src/app/ride.c
    src/app/sec.c
    src/app/speed.c
    src/app/tasks.c
    src/app/sys_state.c
    src/app/volt.c

    # BSP layer
    src/bsp/bmi088.c
    src/bsp/bmp581.c
    src/bsp/osh_phy.c
    src/bsp/sensor_generic.c
    src/bsp/sensor.c
    src/bsp/board.c
    src/bsp/usart.c
)
target_include_directories(application PRIVATE include
                                               ${APP_GENCODE_DIR})
target_link_libraries(application PRIVATE   samd21e18a
                                            uds-lib
                                            flash-line-api flash-line-sources
                                            protocol-stack-api
                                            bootloader-api
                                            bmp5x
                                            bmi08x)

set_target_properties(application PROPERTIES SUFFIX ".elf")
target_link_options(application PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/ld/application.ld)
target_link_options(application PRIVATE -Wl,-Map=application.map -Wl,--gc-sections)

add_library(test-lib INTERFACE)
target_include_directories(test-lib INTERFACE include lib/bmp5x)

# Post processing
hexify(TARGET application HEX ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex)
dump_symbols(application ${CMAKE_CURRENT_BINARY_DIR}/application.lss)
dump_size(application)

####################################################################################################
# Bootloader
####################################################################################################
bootloader_header(
    TARGET factory_header
    CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/tools/bootloader/boot_config.json
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/tools/openocd/factory_header.hex
)

bootloader_header(
    TARGET app_header
    CONFIG ${CMAKE_CURRENT_SOURCE_DIR}/tools/bootloader/boot_config.json
    APPLICATION ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/app_header.hex
)

add_custom_target(application-merge
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/app_header.hex ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex
    BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/application.hex
    COMMAND srec_cat
            ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex -intel
            ${CMAKE_CURRENT_BINARY_DIR}/app_header.hex -intel
            -o ${CMAKE_CURRENT_BINARY_DIR}/application.hex -intel
)

add_custom_target(application-full
    DEPENDS application-hex application-lss application-memory application-merge factory_header
)

#####################################################################################################
# Tests
#####################################################################################################
if (BUILD_SIL_TESTS)
    add_subdirectory(tests/sil)
endif()
if (BUILD_UNIT_TESTS)
    add_subdirectory(tests/unit)
endif()
