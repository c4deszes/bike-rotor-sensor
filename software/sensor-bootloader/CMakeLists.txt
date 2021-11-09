project(
    rotor-sensor-bootloader
    DESCRIPTION "Rotor Sensor UART Bootloader"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.1.0")

# Bootloader API allows applications to interface with the bootloader
add_library(${PROJECT_NAME}-api INTERFACE)
target_include_directories(${PROJECT_NAME}-api INTERFACE api)
target_link_directories(${PROJECT_NAME}-api INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/ld)
set_target_properties(${PROJECT_NAME}-api PROPERTIES BOOTLOADER_LINKERSCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/ld/memory.ld")

# Bootloader
add_executable(
    ${PROJECT_NAME} EXCLUDE_FROM_ALL
    src/bootloader.c
    src/main.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-api rotor-sensor-bsp tiny-hal)
set(APPLICATION_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ld/bootloader.ld)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS
        "$<TARGET_PROPERTY:rotor-sensor-bootloader-api,BOOTLOADER_LINKERSCRIPT>;${APPLICATION_LINKER_SCRIPT}")
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-T${APPLICATION_LINKER_SCRIPT}")
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-Map=${PROJECT_NAME}.map" "-Wl,--gc-sections")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    enable_lto(${PROJECT_NAME})
endif()

# Creates binary output from ELF
add_custom_command(
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    OUTPUT ${PROJECT_NAME}.bin
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating binary output (.bin)")

# Creates hex output from ELF
#   The cached value is needed because the merge function is used outside of this context
set(BOOTLOADER_HEX "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex" CACHE INTERNAL "BOOTLOADER_HEX")
add_custom_command(
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${BOOTLOADER_HEX}
    OUTPUT ${BOOTLOADER_HEX}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating hex output (.hex)")
add_custom_target(${PROJECT_NAME}-hex DEPENDS ${BOOTLOADER_HEX})

# This function allows applications to merge the bootloader into a single hex file
#
# Usage:
#   bootloader_merge(merge-application application.hex application-merged.hex)
function(bootloader_merge TARGET APPLICATION_HEX MERGED_HEX)
    add_custom_command(
        DEPENDS ${APPLICATION_HEX} rotor-sensor-bootloader-hex
        COMMAND "srec_cat"
            ${APPLICATION_HEX} --intel
            ${BOOTLOADER_HEX} --intel
            --output ${MERGED_HEX} --intel
        OUTPUT ${MERGED_HEX}
        COMMENT "\tMerging bootloader into ${MERGED_HEX}"
    )
    add_custom_target(${TARGET} DEPENDS ${MERGED_HEX})
endfunction()

# Creates extended listing file from ELF
add_custom_command(
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJDUMP} -h -S ${PROJECT_NAME}.elf > ${PROJECT_NAME}.lss
    OUTPUT ${PROJECT_NAME}.lss
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating extended listing (.lss)")

# Prints size information of the ELF
add_custom_command(
    DEPENDS ${PROJECT_NAME}
    COMMAND ${CMAKE_OBJSIZE} ${PROJECT_NAME}.elf
    OUTPUT ${PROJECT_NAME}-memory
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "\tCreating memory usage report")

add_custom_target(
    ${PROJECT_NAME}-extras
    DEPENDS
        ${PROJECT_NAME}-memory
        ${PROJECT_NAME}.hex
        ${PROJECT_NAME}.bin
        ${PROJECT_NAME}.lss)
