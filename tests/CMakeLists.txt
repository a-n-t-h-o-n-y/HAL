cmake_minimum_required(VERSION 3.14)

add_executable(tests
    curried.test.cpp
)

target_link_libraries(tests
    PUBLIC
        catch_two
)

add_test(NAME tests COMMAND tests)

# compiler flags/standard version/linking
