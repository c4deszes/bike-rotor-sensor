project(rotor-sensor
    DESCRIPTION "Rotor Sensor Application"
    LANGUAGES C CXX ASM)
set(PROJECT_VERSION "0.1.0")

add_library(${PROJECT_NAME}-api INTERFACE)
target_include_directories(${PROJECT_NAME}-api INTERFACE include)
set(APP_SOURCES
    src/main.c
    src/init.c
    src/tasks.c
    src/dsa.c
    src/pse.c
    src/spe.c
)

add_library(
    ${PROJECT_NAME}-objlib OBJECT EXCLUDE_FROM_ALL
    ${APP_SOURCES}
)
target_link_libraries(${PROJECT_NAME}-objlib PUBLIC tiny-hal rotor-sensor-bsp ${PROJECT_NAME}-api uart-bootloader-api)
if (NOT ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    enable_lto(${PROJECT_NAME}-objlib)
endif()

# Application with Bootloader integrated
#
set(BOOTLOADER_PROJECT ${PROJECT_NAME}-app)
add_executable(
    ${BOOTLOADER_PROJECT}
    $<TARGET_OBJECTS:${PROJECT_NAME}-objlib>
)
target_link_libraries(${BOOTLOADER_PROJECT} PUBLIC tiny-hal rotor-sensor-bsp ${PROJECT_NAME}-api uart-bootloader-api)
set_target_properties(${BOOTLOADER_PROJECT} PROPERTIES SUFFIX ".elf")
set(APPLICATION_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ld/application-bootloader.ld)
set_target_properties(${BOOTLOADER_PROJECT} PROPERTIES LINK_DEPENDS
    "$<TARGET_PROPERTY:uart-bootloader-api,BOOTLOADER_LINKERSCRIPT>;${APPLICATION_LINKER_SCRIPT}")
target_link_options(${BOOTLOADER_PROJECT} PRIVATE "-Wl,-T${APPLICATION_LINKER_SCRIPT}")
target_link_options(${BOOTLOADER_PROJECT} PRIVATE "-Wl,-Map=${BOOTLOADER_PROJECT}.map" "-Wl,--gc-sections")

bootloader_merge(${BOOTLOADER_PROJECT}-merged ${BOOTLOADER_PROJECT}.hex ${BOOTLOADER_PROJECT}-merged.hex)
hexify(${BOOTLOADER_PROJECT} ${BOOTLOADER_PROJECT}.hex)
dump_symbols(${BOOTLOADER_PROJECT} ${BOOTLOADER_PROJECT}.lss)
add_custom_target(
    ${BOOTLOADER_PROJECT}-full
    DEPENDS
    ${BOOTLOADER_PROJECT}-hex
    ${BOOTLOADER_PROJECT}-lss
    ${BOOTLOADER_PROJECT}-merged
)

# Standalone application
#
set(STANDALONE_PROJECT ${PROJECT_NAME}-standalone)
add_executable(
    ${STANDALONE_PROJECT}
    $<TARGET_OBJECTS:${PROJECT_NAME}-objlib>
)
target_link_libraries(${STANDALONE_PROJECT} PUBLIC tiny-hal rotor-sensor-bsp ${PROJECT_NAME}-api uart-bootloader-api)
set_target_properties(${STANDALONE_PROJECT} PROPERTIES SUFFIX ".elf")
set(APPLICATION_LINKER_SCRIPT_STANDALONE ${CMAKE_CURRENT_SOURCE_DIR}/ld/application.ld)
set_target_properties(${STANDALONE_PROJECT} PROPERTIES LINK_DEPENDS ${APPLICATION_LINKER_SCRIPT_STANDALONE})
target_link_options(${STANDALONE_PROJECT} PRIVATE "-Wl,-T${APPLICATION_LINKER_SCRIPT_STANDALONE}")
target_link_options(${STANDALONE_PROJECT} PRIVATE "-Wl,-Map=${STANDALONE_PROJECT}.map" "-Wl,--gc-sections")

hexify(${STANDALONE_PROJECT} ${STANDALONE_PROJECT}.hex)
dump_symbols(${STANDALONE_PROJECT} ${STANDALONE_PROJECT}.lss)
dump_size(${STANDALONE_PROJECT})
add_custom_target(
    ${STANDALONE_PROJECT}-full
    DEPENDS
    ${STANDALONE_PROJECT}-hex
    ${STANDALONE_PROJECT}-lss
    ${STANDALONE_PROJECT}-memory
)

add_library(
    rotor-sensor-test STATIC EXCLUDE_FROM_ALL
    ${APP_SOURCES}
)
target_link_libraries(rotor-sensor-test PUBLIC rotor-sensor-api rotor-sensor-bsp-test)

add_subdirectory(test/sil)
add_subdirectory(test/unit)
