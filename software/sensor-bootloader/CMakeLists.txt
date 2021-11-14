project(
    uart-bootloader
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
    src/tasks.c
)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")
target_include_directories(${PROJECT_NAME} PRIVATE include)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}-api rotor-sensor-bsp tiny-hal)
set(APPLICATION_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ld/bootloader.ld)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_DEPENDS
        "$<TARGET_PROPERTY:${PROJECT_NAME}-api,BOOTLOADER_LINKERSCRIPT>;${APPLICATION_LINKER_SCRIPT}")
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-T${APPLICATION_LINKER_SCRIPT}")
target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-Map=${PROJECT_NAME}.map" "-Wl,--gc-sections")

if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    enable_lto(${PROJECT_NAME})
endif()

# Creates hex output from ELF
#   The cached value is needed because the merge function is used outside of this context
set(BOOTLOADER_HEX "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.hex" CACHE INTERNAL "BOOTLOADER_HEX")
hexify(${PROJECT_NAME} ${BOOTLOADER_HEX})

dump_symbols(${PROJECT_NAME} ${PROJECT_NAME}.lss)
dump_size(${PROJECT_NAME})
add_custom_target(
    ${PROJECT_NAME}-full
    DEPENDS
    ${PROJECT_NAME}-hex
    ${PROJECT_NAME}-lss
    ${PROJECT_NAME}-memory
)

# This function allows applications to merge the bootloader into a single hex file
# Usage:
#   bootloader_merge(merge-application application.hex application-merged.hex)
# Prerequisite:
#   SRecord (srec_cat) must be on the path
function(bootloader_merge TARGET APPLICATION_HEX MERGED_HEX)
    add_custom_command(
        DEPENDS ${APPLICATION_HEX} uart-bootloader-hex
        COMMAND "srec_cat"
            ${APPLICATION_HEX} --intel
            ${BOOTLOADER_HEX} --intel
            --output ${MERGED_HEX} --intel
        OUTPUT ${MERGED_HEX}
        COMMENT "\tMerging bootloader into ${MERGED_HEX}"
    )
    add_custom_target(${TARGET} DEPENDS ${MERGED_HEX})
endfunction()
