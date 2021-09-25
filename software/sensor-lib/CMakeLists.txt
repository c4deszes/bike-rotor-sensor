add_library(rotor-sensor-lib STATIC EXCLUDE_FROM_ALL
    src/dsa.c
    src/pse.c
    src/spe.c
)
target_include_directories(rotor-sensor-lib PUBLIC api)
target_link_libraries(rotor-sensor-lib PRIVATE rotor-sensor-lib-cfg)

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    enable_lto(rotor-sensor-lib)
endif()

add_subdirectory(test/unit)
