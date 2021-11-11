project(
    rotor-sensor
    DESCRIPTION "Rotor Sensor Application"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.1.0")

add_executable(
    ${PROJECT_NAME} EXCLUDE_FROM_ALL
    src/main.c
    src/init.c
    src/tasks.c
    src/dsa.c
    src/pse.c
    src/spe.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE rotor-sensor-bsp tiny-hal)

if (APP_BOOTLOADER_ENABLED)
    target_link_libraries(${PROJECT_NAME} PRIVATE rotor-sensor-bootloader-api)
    set(APPLICATION_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ld/application-bootloader.ld)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS
        "$<TARGET_PROPERTY:rotor-sensor-bootloader-api,BOOTLOADER_LINKERSCRIPT>;${APPLICATION_LINKER_SCRIPT}")
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-T${APPLICATION_LINKER_SCRIPT}")
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-Map=${PROJECT_NAME}.map" "-Wl,--gc-sections")
    bootloader_merge(rotor-sensor-merged ${PROJECT_NAME}.hex ${PROJECT_NAME}-merged.hex)
else()
    set(APPLICATION_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ld/application.ld)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS ${APPLICATION_LINKER_SCRIPT})
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-T${APPLICATION_LINKER_SCRIPT}")
    target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-Map=${PROJECT_NAME}.map" "-Wl,--gc-sections")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    enable_lto(${PROJECT_NAME})
endif()

# Creates binary output from ELF
add_custom_command(
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    OUTPUT ${PROJECT_NAME}.bin
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating binary output (.bin)"
)

# Creates hex output from ELF
add_custom_command(
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    OUTPUT ${PROJECT_NAME}.hex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating hex output (.hex)"
)

# Creates extended listing file from ELF
add_custom_command(
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJDUMP} -h -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lss
    OUTPUT ${PROJECT_NAME}.lss
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating extended listing (.lss)"
)

# Prints size information of the ELF
add_custom_command(
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJSIZE} ${PROJECT_NAME}.elf
    OUTPUT ${PROJECT_NAME}-memory
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating memory usage report"
)

add_custom_target(
    ${PROJECT_NAME}-extras
    DEPENDS
        ${PROJECT_NAME}-memory
        ${PROJECT_NAME}.hex
        ${PROJECT_NAME}.bin
        ${PROJECT_NAME}.lss
)
