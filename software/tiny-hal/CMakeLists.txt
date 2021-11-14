add_library(tiny-hal-api INTERFACE)
target_include_directories(tiny-hal-api INTERFACE api)

add_library(
    tiny-hal STATIC EXCLUDE_FROM_ALL
    src/ac.c
    src/adc.c
    src/cpu.c
    src/cpuint.c
    src/crcscan.c
    src/clkctrl.c
    src/evsys.c
    src/nvmctrl.c
    src/port.c
    src/portmux.c
    src/rstctrl.c
    src/rtc.c
    src/sch.c
    src/slpctrl.c
    src/sys.c
    src/tca.c
    src/tcb.c
    src/usart.c
    src/vref.c
    src/wdt.c
)
target_link_libraries(tiny-hal PUBLIC tiny-hal-api tiny-hal-cfg)

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    enable_lto(tiny-hal)
endif()

add_subdirectory(fake)
