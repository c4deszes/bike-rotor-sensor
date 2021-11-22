add_executable(
    rotor-sensor-unittest EXCLUDE_FROM_ALL
    src/main.cpp
    src/test_pse.cpp
    src/test_dsa.cpp
)
target_link_libraries(rotor-sensor-unittest PUBLIC rotor-sensor-test gcc gtest)
target_link_options(rotor-sensor-unittest PUBLIC -static)
gtest_discover_tests(rotor-sensor-unittest)
