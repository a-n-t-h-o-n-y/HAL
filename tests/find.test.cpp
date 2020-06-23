#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("find_if", "[HAL]")
{
    SECTION("Full Call")
    {
        CHECK(hal::find_if([](auto x) { return x > 4; }, 1, 2, 3, 4, 5, 6) ==
              4);
        CHECK(hal::find_if([](auto x) { return x > 0; }, 1, 2, 3, 4, 5, 6) ==
              0);
        CHECK(hal::find_if([](auto x) { return x > 100; }, 1, 2, 3, 4, 5, 6) ==
              6);
    }
    SECTION("Partial Application")
    {
        auto find_gt_4 = hal::find_if([](auto x) { return x > 4; });
        CHECK(find_gt_4(1, 2, 3, 4, 5, 6) == 4);
        CHECK(find_gt_4(0, 0, 0, 0, 0, 0) == 6);
        CHECK(find_gt_4(5) == 0);
    }
    SECTION("constexpr")
    {
        static_assert(
            hal::find_if([](auto x) { return x > 4; }, 1, 2, 3, 4, 5, 6) == 4);
        static_assert(
            hal::find_if([](auto x) { return x > 0; }, 1, 2, 3, 4, 5, 6) == 0);
        static_assert(hal::find_if([](auto x) { return x > 100; }, 1, 2, 3, 4,
                                   5, 6) == 6);
    }
}

TEST_CASE("reverse::find_if", "[HAL]")
{
    SECTION("Full Call")
    {
        CHECK(hal::reverse::find_if([](auto x) { return x > 4; }, 1, 2, 3, 4, 5,
                                    6) == 5);
        CHECK(hal::reverse::find_if([](auto x) { return x == 2; }, 1, 2, 2, 3,
                                    4, 5, 6) == 2);
        CHECK(hal::reverse::find_if([](auto x) { return x > 100; }, 1, 2, 3, 4,
                                    5, 6) == 6);
    }
    SECTION("Partial Application")
    {
        auto rev_find_gt_4 =
            hal::reverse::find_if([](auto x) { return x > 4; });
        CHECK(rev_find_gt_4(1, 2, 3, 4, 5, 6) == 5);
        CHECK(rev_find_gt_4(0, 0, 0, 0, 0, 0) == 6);
        CHECK(rev_find_gt_4(5) == 0);
    }
    SECTION("constexpr")
    {
        constexpr auto i = hal::reverse::find_if([](auto x) { return x > 4; },
                                                 1, 2, 3, 4, 5, 6);
        CHECK(i == 5);
        static_assert(hal::reverse::find_if([](auto x) { return x > 4; }, 1, 2,
                                            3, 4, 5, 6) == 5);
        static_assert(hal::reverse::find_if([](auto x) { return x == 2; }, 1, 2,
                                            2, 3, 4, 5, 6) == 2);
        static_assert(hal::reverse::find_if([](auto x) { return x > 100; }, 1,
                                            2, 3, 4, 5, 6) == 6);
    }
}

TEST_CASE("find_if_not", "[HAL]")
{
    SECTION("Full Call")
    {
        CHECK(hal::find_if_not([](auto x) { return x > 4; }, 1, 2, 3, 4, 5,
                               6) == 0);
        CHECK(hal::find_if_not([](auto x) { return x > 0; }, 1, 2, 3, -4, 5,
                               0) == 3);
        CHECK(hal::find_if_not([](auto x) { return x < 100; }, 1, 2, 3, 4, 5,
                               6) == 6);
    }
    SECTION("Partial Application")
    {
        auto find_if_not_gt_4 = hal::find_if_not([](auto x) { return x > 4; });
        CHECK(find_if_not_gt_4(1, 2, 3, 4, 5, 6) == 0);
        CHECK(find_if_not_gt_4(5, 5, 5) == 3);
        CHECK(find_if_not_gt_4(0) == 0);
        CHECK(find_if_not_gt_4('a') == 1);
    }
    SECTION("constexpr")
    {
        static_assert(hal::find_if_not([](auto x) { return x > 4; }, 1, 2, 3, 4,
                                       5, 6) == 0);
        static_assert(hal::find_if_not([](auto x) { return x > 0; }, 1, 2, 3,
                                       -4, 5, 0) == 3);
        static_assert(hal::find_if_not([](auto x) { return x < 100; }, 1, 2, 3,
                                       4, 5, 6) == 6);
    }
}

TEST_CASE("reverse::find_if_not", "[HAL]")
{
    SECTION("Full Call")
    {
        CHECK(hal::reverse::find_if_not([](auto x) { return x > 4; }, 1, 2, 3,
                                        4, 5, 6) == 3);
        CHECK(hal::reverse::find_if_not([](auto x) { return x > 0; }, 1, 2, 3,
                                        -4, 5, 0) == 5);
        CHECK(hal::reverse::find_if_not([](auto x) { return x < 100; }, 1, 2, 3,
                                        4, 5, 6) == 6);
    }
    SECTION("Partial Application")
    {
        auto rev_find_if_not_gt_4 =
            hal::reverse::find_if_not([](auto x) { return x > 4; });
        CHECK(rev_find_if_not_gt_4(1, 2, 3, 4, 5, 6) == 3);
        CHECK(rev_find_if_not_gt_4(5, 5, 5) == 3);
        CHECK(rev_find_if_not_gt_4(0) == 0);
        CHECK(rev_find_if_not_gt_4('a') == 1);
    }
    SECTION("constexpr")
    {
        static_assert(hal::reverse::find_if_not([](auto x) { return x > 4; }, 1,
                                                2, 3, 4, 5, 6) == 3);
        static_assert(hal::reverse::find_if_not([](auto x) { return x > 0; }, 1,
                                                2, 3, -4, 5, 0) == 5);
        static_assert(hal::reverse::find_if_not([](auto x) { return x < 100; },
                                                1, 2, 3, 4, 5, 6) == 6);
    }
}

TEST_CASE("find", "[HAL]")
{
    SECTION("Full Call")
    {
        CHECK(hal::find(1, 1, 2, 3, 4, 5, 6) == 0);
        CHECK(hal::find(6, 1, 2, 3, 4, 5, 6) == 5);
        CHECK(hal::find('a', 1, 2, 3, 4, 5, 6) == 6);
    }
    SECTION("Partial Application")
    {
        auto find_1 = hal::find(1);
        CHECK(find_1(1, 2, 3, 4, 5, 6) == 0);
        CHECK(find_1(0, 0, 0, 0, 0) == 5);
        CHECK(find_1(0, 1, 2, 3, 4, 1, 5, 6) == 1);
    }
    SECTION("constexpr")
    {
        static_assert(hal::find(1, 1, 2, 3, 4, 5, 6) == 0);
        static_assert(hal::find(6, 1, 2, 3, 4, 5, 6) == 5);
        static_assert(hal::find('a', 1, 2, 3, 4, 5, 6) == 6);
    }
}

TEST_CASE("reverse::find", "[HAL]")
{
    SECTION("Full Call")
    {
        CHECK(hal::reverse::find(1, 1, 'b', 3, 4, 1, 5, 6) == 4uL);
        CHECK(hal::reverse::find('z', 1, 'z', 2, 3, 4, 'z', 5, 6) == 5);
        CHECK(hal::reverse::find('a', 1, 2, 3, 4, 5, 6) == 6);
        CHECK(hal::reverse::find(1, 1, 'b', 3, 4, 5, 6) == 0);
    }
    SECTION("Partial Application")
    {
        auto rev_find_1 = hal::reverse::find(1);
        CHECK(rev_find_1(1, 2, 3, 4, 1, 5, 6) == 4);
        CHECK(rev_find_1(0, 0, 0, 0, 0) == 5);
        CHECK(rev_find_1(0, 1, 2, 3, 4, 5, 6) == 1);
    }
    SECTION("constexpr")
    {
        static_assert(hal::reverse::find(1, 1, 'b', 3, 4, 1, 5, 6) == 4);
        static_assert(hal::reverse::find('z', 1, 'z', 2, 3, 4, 'z', 5, 6) == 5);
        static_assert(hal::reverse::find('a', 1, 2, 3, 4, 5, 6) == 6);
        static_assert(hal::reverse::find(1, 1, 'b', 3, 4, 5, 6) == 0);
    }
}
