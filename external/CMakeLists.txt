cmake_minimum_required(VERSION 3.5.1)

add_subdirectory(Catch2)

# Catch2 Main
add_library(catch_two catch.main.cpp)
target_link_libraries(catch_two PUBLIC Catch2::Catch2)
