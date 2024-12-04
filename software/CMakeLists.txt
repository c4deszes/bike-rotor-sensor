cmake_minimum_required(VERSION 3.20)

project(
    rotor-sensor
    DESCRIPTION "Rotor Sensor"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.3.0")

find_package(Python REQUIRED)

include(tools/cmake/CPM.cmake)

CPMAddPackage("gh:c4deszes/samd21-hal@0.3.0")
CPMAddPackage("gh:c4deszes/bike-line-protocol@0.1.0")
CPMAddPackage("gh:c4deszes/bike-flash-tool@0.1.0")
CPMAddPackage("gh:c4deszes/samd21-line-bootloader@0.3.0")
CPMAddPackage("gh:c4deszes/bike-uds-tool@0.1.0")

# Test configuration
enable_testing()
include(CTest)
set(CTEST_OUTPUT_ON_FAILURE ON)

include(CTest)
include(GoogleTest)
include(FetchContent)

# Project
add_subdirectory(docs)

line_codegen(
    TARGET protocol-stack-api
    NETWORK ../customer/network.json
    NODE RotorSensor
    ADAPTER
)

uds_codegen(
    TARGET uds-lib
    PROFILE tools/line/uds_profile.json
)

samx21_hal(samd21e18a ATSAMD21E18A)

# BMP581 Pressure sensor library
add_library(
    bmp5 INTERFACE
)
target_sources(bmp5 INTERFACE
    lib/bmp5x/bmp5.c
)
#target_compile_definitions(bmp5 INTERFACE BMP5_USE_FIXED_POINT)
target_include_directories(bmp5 INTERFACE lib/bmp5x)

# BMI088 IMU library
add_library(
    bmi08x INTERFACE
)
target_sources(bmi08x INTERFACE
    lib/bmi08x/bmi08a.c
    lib/bmi08x/bmi08g.c
    lib/bmi08x/bmi08xa.c
)
target_include_directories(bmi08x INTERFACE lib/bmi08x)

add_executable(
    application
    EXCLUDE_FROM_ALL

    src/app/alt.c
    src/app/comm.c
    src/app/iet.c
    src/app/init.c
    src/app/itpms.c
    src/app/main.c
    src/app/sec.c
    src/app/spm.c
    src/app/tasks.c
    src/app/sys_state.c

    src/bsp/bmp581.c
    src/bsp/osh_phy.c
    src/bsp/sensor_generic.c
    src/bsp/sensor.c
    src/bsp/board.c
    src/bsp/usart.c
)
target_include_directories(application PRIVATE include)
target_link_libraries(application PRIVATE   samd21e18a
                                            uds-lib
                                            flash-line-api flash-line-sources
                                            protocol-stack-api
                                            bootloader-api
                                            bmp5
                                            bmi08x)

set_target_properties(application PROPERTIES SUFFIX ".elf")
target_link_options(application PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/ld/application.ld)
target_link_options(application PRIVATE -Wl,-Map=application.map -Wl,--gc-sections)

add_library(test-lib INTERFACE)
target_include_directories(test-lib INTERFACE include lib/bmp5x)

hexify(TARGET application HEX ${CMAKE_CURRENT_BINARY_DIR}/application_noheader.hex)
dump_symbols(application ${CMAKE_CURRENT_BINARY_DIR}/application.lss)
dump_size(application)

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

add_subdirectory(tests/unit)
