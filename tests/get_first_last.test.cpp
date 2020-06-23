#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("first", "[HAL]")
{
    CHECK(hal::first(1, 2, 'a', 4.3) == 1);
    CHECK(hal::first(2.44, 1, 2, 'a', 4.3) == 2.44);
    CHECK(typeid(hal::first()) == typeid(void));

    static_assert(hal::first(1, 2, 'a', 4.3) == 1);
    static_assert(hal::first(2.44, 1, 2, 'a', 4.3) == 2.44);
}

TEST_CASE("last", "[HAL]")
{
    CHECK(hal::last(1, 2, 'a', 4.3) == 4.3);
    CHECK(hal::last(2.44, 1, 2, 'a', 4.3, 'b') == 'b');
    CHECK(typeid(hal::last()) == typeid(void));
    static_assert(hal::last(1, 2, 'a', 4.3) == 4.3);
    static_assert(hal::last(2.44, 1, 2, 'a', 4.3, 'b') == 'b');
}

TEST_CASE("get", "[HAL]")
{
    CHECK(hal::get<2>(1, 2, 'a', 4.3) == 'a');
    CHECK(hal::get<0>(2.44, 1, 2, 'a', 4.3) == 2.44);
    CHECK(hal::get<4>(2.44, 1, 2, 'a', 4.3) == 4.3);
    CHECK(hal::get<1>(2.44, 1, 2, 'a', 4.3) == 1);
    CHECK(typeid(hal::get<5>()) == typeid(void));
    CHECK(typeid(hal::get<0>()) == typeid(void));

    static_assert(hal::get<2>(1, 2, 'a', 4.3) == 'a');
    static_assert(hal::get<0>(2.44, 1, 2, 'a', 4.3) == 2.44);
    static_assert(hal::get<4>(2.44, 1, 2, 'a', 4.3) == 4.3);
    static_assert(hal::get<1>(2.44, 1, 2, 'a', 4.3) == 1);
}
