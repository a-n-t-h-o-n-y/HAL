#include <catch2/catch.hpp>

#include <halg.hpp>

TEST_CASE("first", "[HALG]")
{
    CHECK(halg::first(1, 2, 'a', 4.3) == 1);
    CHECK(halg::first(2.44, 1, 2, 'a', 4.3) == 2.44);
    CHECK(typeid(halg::first()) == typeid(void));

    static_assert(halg::first(1, 2, 'a', 4.3) == 1);
    static_assert(halg::first(2.44, 1, 2, 'a', 4.3) == 2.44);
}

TEST_CASE("last", "[HALG]")
{
    CHECK(halg::last(1, 2, 'a', 4.3) == 4.3);
    CHECK(halg::last(2.44, 1, 2, 'a', 4.3, 'b') == 'b');
    CHECK(typeid(halg::last()) == typeid(void));
    static_assert(halg::last(1, 2, 'a', 4.3) == 4.3);
    static_assert(halg::last(2.44, 1, 2, 'a', 4.3, 'b') == 'b');
}

TEST_CASE("get", "[HALG]")
{
    CHECK(halg::get<2>(1, 2, 'a', 4.3) == 'a');
    CHECK(halg::get<0>(2.44, 1, 2, 'a', 4.3) == 2.44);
    CHECK(halg::get<4>(2.44, 1, 2, 'a', 4.3) == 4.3);
    CHECK(halg::get<1>(2.44, 1, 2, 'a', 4.3) == 1);
    CHECK(typeid(halg::get<5>()) == typeid(void));
    CHECK(typeid(halg::get<0>()) == typeid(void));

    static_assert(halg::get<2>(1, 2, 'a', 4.3) == 'a');
    static_assert(halg::get<0>(2.44, 1, 2, 'a', 4.3) == 2.44);
    static_assert(halg::get<4>(2.44, 1, 2, 'a', 4.3) == 4.3);
    static_assert(halg::get<1>(2.44, 1, 2, 'a', 4.3) == 1);
}
