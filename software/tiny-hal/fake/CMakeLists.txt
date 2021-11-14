add_library(tiny-hal-fake STATIC EXCLUDE_FROM_ALL
    src/scheduler.cpp
    src/peripheral.cpp
)
target_include_directories(tiny-hal-fake PUBLIC include)
target_link_libraries(tiny-hal-fake PUBLIC tiny-hal-api)
