add_executable(
    rotor-sensor-test
    src/main.cpp
    src/test_pse.cpp
)
target_link_libraries(rotor-sensor-test PUBLIC rotor-sensor-lib gcc gtest)
target_link_options(rotor-sensor-test PUBLIC -static)
gtest_discover_tests(rotor-sensor-test)
