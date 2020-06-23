cmake_minimum_required(VERSION 3.14)
Project(HAL LANGUAGES CXX)
enable_testing()

add_library(hal INTERFACE)

target_include_directories(hal
    INTERFACE
        include/
)

target_compile_features(hal
    INTERFACE
        cxx_std_17
)

target_compile_options(hal
    INTERFACE
        -Wall
        -Wextra
)

add_subdirectory(external)
add_subdirectory(tests)
