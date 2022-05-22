cmake_minimum_required(VERSION 3.20)

project(
    rotor-sensor
    DESCRIPTION "Rotor Sensor"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.1.0")

# Profile selection
message("-- Rotor Sensor profile: internal-optical-digital")
include("config/internal-optical-digital.cmake")
message("\tBoard type: ${BOARD_TYPE}")
message("\tBoard sensor type: ${BOARD_SENSOR_TYPE}")

# Test configuration
enable_testing()
include(CTest)
set(CTEST_OUTPUT_ON_FAILURE ON)

set(CTEST_COVERAGE_COMMAND "gcovr")
set(CTEST_COVERAGE_EXTRA_FLAGS "--xml ${CMAKE_CURRENT_BINARY_DIR}/coverage.xml")

include(CTest)
include(GoogleTest)
add_subdirectory("ext/googletest")

# Linting configuration
#set(CMAKE_C_CPPLINT "cpplint")

# Tools
include(tools/cmake/application.cmake)
include(tools/cmake/gcc-lto.cmake)

# Project
add_subdirectory(docs)
add_subdirectory(sensor-bootloader)
add_subdirectory(sensor-app)
add_subdirectory(sensor-bsp)
add_subdirectory(tiny-hal)
