add_library(rotor-sensor-lib-cfg INTERFACE)
target_link_libraries(rotor-sensor-lib-cfg INTERFACE tiny-hal-cfg)

add_executable(
    rotor-sensor EXCLUDE_FROM_ALL
    src/main.c
    src/init.c
    src/signals.c
    src/tasks.c
)
set_target_properties(rotor-sensor PROPERTIES SUFFIX ".elf")
target_include_directories(rotor-sensor PRIVATE include)
target_link_libraries(rotor-sensor PRIVATE rotor-sensor-lib rotor-sensor-bsp tiny-hal)
target_link_options(rotor-sensor PRIVATE "-Wl,-Map=${PROJECT_NAME}.map" "-Wl,--gc-sections")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    enable_lto(rotor-sensor)
endif()

# Creates binary output from ELF
add_custom_command(
    DEPENDS rotor-sensor
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    OUTPUT ${PROJECT_NAME}.bin
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating binary output (.bin)"
)

# Creates hex output from ELF
add_custom_command(
    DEPENDS rotor-sensor
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    OUTPUT ${PROJECT_NAME}.hex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating hex output (.hex)"
)

# Creates extended listing file from ELF
add_custom_command(
    DEPENDS rotor-sensor
    COMMAND ${CMAKE_OBJDUMP} -h -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lss
    OUTPUT ${PROJECT_NAME}.lss
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating extended listing (.lss)"
)

# Prints size information of the ELF
add_custom_command(
    DEPENDS rotor-sensor
    COMMAND ${CMAKE_OBJSIZE} ${PROJECT_NAME}.elf
    OUTPUT ${PROJECT_NAME}-memory
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating memory usage report"
)

add_custom_target(
    rotor-sensor-extras
    DEPENDS
        ${PROJECT_NAME}-memory
        ${PROJECT_NAME}.hex
        ${PROJECT_NAME}.bin
        ${PROJECT_NAME}.lss
)
