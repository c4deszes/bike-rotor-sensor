cmake_minimum_required(VERSION 3.20)

project(
    rotor-sensor
    DESCRIPTION "Rotor Sensor"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.2.0")

# Test configuration
enable_testing()
include(CTest)
set(CTEST_OUTPUT_ON_FAILURE ON)

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

add_library(cmsis INTERFACE)
target_include_directories(cmsis INTERFACE lib/CMSIS/CMSIS/Core/Include)

add_library(atsamd21e18a-dfp INTERFACE)
target_include_directories(atsamd21e18a-dfp INTERFACE lib/ATSAMD21E18A_DFP)

add_executable(
    application
    EXCLUDE_FROM_ALL

    src/app/main.c
    src/app/sch.c
    src/app/init.c
    src/app/osh.c
    src/app/tasks.c

    src/bsp/osh_max9621.c
    src/bsp/osh_phy.c
    src/bsp/sensor_generic.c
    src/bsp/sensor.c
    src/bsp/board.c

    src/hal/eic.c
    src/hal/gclk.c
    src/hal/gpio.c
    src/hal/pm.c
    src/hal/evsys.c
    src/hal/rtc.c
    src/hal/nvic.c
    src/hal/tc.c
    src/hal/wdt.c
    src/hal/startup.c
    src/hal/sysctrl.c
)
target_include_directories(application PRIVATE include)
target_link_libraries(application PRIVATE cmsis atsamd21e18a-dfp)
target_compile_options(application PRIVATE "-mcpu=cortex-m0" "-gdwarf-2" "-mthumb")
set_target_properties(application PROPERTIES SUFFIX ".elf")
target_link_options(application PRIVATE "-mcpu=cortex-m0" "-mthumb")
target_link_options(application PRIVATE -T${CMAKE_CURRENT_SOURCE_DIR}/ld/ATSAMD21E18A.ld)
target_link_options(application PRIVATE -Wl,-Map=application.map -Wl,--gc-sections)

hexify(application ${CMAKE_CURRENT_BINARY_DIR}/application.hex)
dump_symbols(application ${CMAKE_CURRENT_BINARY_DIR}/application.lss)
dump_size(application)

add_custom_target(application-full
    DEPENDS application-hex application-lss application-memory
)