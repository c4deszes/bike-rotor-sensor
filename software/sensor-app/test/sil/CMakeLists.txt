add_executable(
    rotor-sensor-siltest EXCLUDE_FROM_ALL
    src/main.cpp
    src/test_sensor.cpp
)
target_link_libraries(rotor-sensor-siltest PUBLIC rotor-sensor-test gcc gtest)
target_link_options(rotor-sensor-siltest PUBLIC -static)
gtest_discover_tests(rotor-sensor-siltest)
