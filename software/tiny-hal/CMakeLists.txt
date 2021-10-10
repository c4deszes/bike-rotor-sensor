add_library(
    tiny-hal STATIC EXCLUDE_FROM_ALL
    src/ac.c
    src/adc.c
    src/cpu.c
    src/clkctrl.c
    src/evsys.c
    src/port.c
    src/portmux.c
    src/rstctrl.c
    src/sch.c
    src/slpctrl.c
    src/sys.c
    src/tca.c
    src/tcb.c
    src/usart.c
    src/vref.c
    src/wdt.c
)
target_include_directories(tiny-hal PUBLIC api)
target_link_libraries(tiny-hal PUBLIC tiny-hal-cfg)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    enable_lto(tiny-hal)
endif()
