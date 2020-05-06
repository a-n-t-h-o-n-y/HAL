cmake_minimum_required(VERSION 3.14)

add_executable(tests
    tests.cpp
)

target_link_libraries(tests
    PUBLIC
        halg
        catch_two
)

add_test(NAME tests COMMAND tests)
