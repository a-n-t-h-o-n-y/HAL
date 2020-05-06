cmake_minimum_required(VERSION 3.14)
Project(HALG LANGUAGES CXX)
enable_testing()

add_library(halg INTERFACE)

target_include_directories(halg
    INTERFACE
        include/
)

target_compile_features(halg
    INTERFACE
        cxx_std_17
)

target_compile_options(halg
    INTERFACE
        -Wall
        -Wextra
)

add_subdirectory(external)
add_subdirectory(tests)
