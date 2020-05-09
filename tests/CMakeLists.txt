cmake_minimum_required(VERSION 3.14)

add_executable(tests
    for_each.test.cpp
    reduce.test.cpp
    transform.test.cpp
    transform_reduce.test.cpp
    count.test.cpp
    all_any_none_of.test.cpp
)

target_link_libraries(tests
    PUBLIC
        halg
        catch_two
)

add_test(NAME tests COMMAND tests)
