FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.12.0
)
FetchContent_Declare(
    fff
    GIT_REPOSITORY https://github.com/meekrosoft/fff.git
    GIT_TAG        5111c61e1ef7848e3afd3550044a8cf4405f4199
)

FetchContent_MakeAvailable(googletest fff)

function(add_test TARGET)
    target_compile_options(${TARGET} PRIVATE $<$<COMPILE_LANGUAGE:C>:--coverage>)
    #target_link_libraries(${TARGET} PRIVATE line-protocol-api)
    target_link_libraries(${TARGET} PRIVATE gtest fff gcov)
    gtest_discover_tests(${TARGET} DISCOVERY_MODE POST_BUILD)
endfunction()

add_executable(test_Altitude EXCLUDE_FROM_ALL)
target_sources(test_Altitude PRIVATE
    ../../src/app/alt.c
    test_Altitude.cpp
)
target_link_libraries(test_Altitude PRIVATE test-lib samx21-api)
add_test(test_Altitude)
