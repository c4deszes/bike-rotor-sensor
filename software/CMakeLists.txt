cmake_minimum_required(VERSION 3.20)

project(
    rotor-sensor
    DESCRIPTION "Rotor Sensor"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.3.0")

message("-- Rotor Sensor profile: ${PROJECT_PROFILE}")
include(profiles/${PROJECT_PROFILE}.cmake)
message("\tBoard sensor driver: ${BOARD_SENSOR_DRIVER}")

find_package(Python REQUIRED)

include(tools/cmake/CPM.cmake)

CPMAddPackage("gh:c4deszes/samd21-hal@0.2.0")
CPMAddPackage("gh:c4deszes/bike-line-protocol#master")
CPMAddPackage("gh:c4deszes/bike-flash-tool@0.1.0")
CPMAddPackage("gh:c4deszes/samd21-line-bootloader@0.2.0")

# Test configuration
enable_testing()
include(CTest)
set(CTEST_OUTPUT_ON_FAILURE ON)

include(CTest)
include(GoogleTest)

# Project
add_subdirectory(docs)

line_codegen(
    TARGET protocol-stack-api
    NETWORK ../customer/network.json
    NODE RotorSensor
    ADAPTER
)

add_executable(
    application
    EXCLUDE_FROM_ALL

    src/app/comm.c
    src/app/iet.c
    src/app/init.c
    src/app/itpms.c
    src/app/main.c
    src/app/sec.c
    src/app/spm.c
    src/app/tasks.c
    src/app/user_conf.c
    src/app/sys_state.c

    $<$<STREQUAL:${BOARD_SENSOR_DRIVER},MAX9621>:src/bsp/osh_max9621.c>
    $<$<STREQUAL:${BOARD_SENSOR_DRIVER},AP22802>:src/bsp/osh_ap22802.c>
    src/bsp/osh_phy.c
    src/bsp/sensor_generic.c
    src/bsp/sensor.c
    src/bsp/board.c
    src/bsp/usart.c
)
target_include_directories(application PRIVATE include)
target_link_libraries(application PRIVATE   cmsis
                                            atsamd21e18a-dfp
                                            samd21-hal-api
                                            samd21-hal-sources
                                            samd21-hal-options
                                            flash-line-api flash-line-sources
                                            protocol-stack-api
                                            bootloader-api)

set_target_properties(application PROPERTIES SUFFIX ".elf")
target_link_options(application PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/ld/application.ld)
target_link_options(application PRIVATE -Wl,-Map=application.map -Wl,--gc-sections)

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
