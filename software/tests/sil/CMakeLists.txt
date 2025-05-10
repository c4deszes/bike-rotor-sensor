####################################################################################################
# Dependencies
####################################################################################################
CPMAddPackage("gh:capnproto/capnproto@1.1.0")

####################################################################################################
# Code generation
####################################################################################################
capnp_generate_cpp(rotorsensor-rpc-sources rotorsensor-rpc-headers server/rotor_sensor.capnp)

####################################################################################################
# Software in loop (SIL) test application
####################################################################################################
add_executable(rotorsensor-server EXCLUDE_FROM_ALL
    ${rotorsensor-rpc-sources}
    server/server.cpp
    ../../src/app/altitude.c
    ../../src/app/cadence.c
    # ../../src/app/config.c
    ../../src/app/current.c
    ../../src/app/distance.c
    ../../src/app/gear.c
    ../../src/app/itpms.c
    ../../src/app/imu.c
    ../../src/app/init.c
    ../../src/app/ride.c
    ../../src/app/sec.c
    ../../src/app/speed.c
    ../../src/app/sys_state.c
    ../../src/app/tasks.c
    ../../src/app/volt.c
    stubs/bmp5.c
    stubs/swtimer.c
)
target_include_directories(rotorsensor-server PRIVATE
    $<PATH:REMOVE_FILENAME,${rotorsensor-rpc-headers}>     # TODO: why is this needed?
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/stubs)

target_link_libraries(rotorsensor-server PRIVATE
    bmp5x-api
    bmi08x-api
    samx21-api
    #uds-lib
    bootloader-api)

target_link_libraries(rotorsensor-server PRIVATE CapnProto::capnp CapnProto::capnp-rpc)
target_compile_options(rotorsensor-server PRIVATE -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-missing-field-initializers)
