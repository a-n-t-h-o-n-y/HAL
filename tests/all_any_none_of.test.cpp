#include <sstream>

#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("all_of", "[HAL]")
{
    SECTION("full call")
    {
        CHECK(!hal::all_of([](auto x) { return x; }, true, false, true));
        CHECK(hal::all_of([](auto x) { return x; }, true));
        CHECK(hal::all_of([](auto x) { return x > 42; }, 54.3, 100.23, 'A'));
        CHECK(!hal::all_of([](auto x) { return x > 42; }, 54.3, -1, 'A'));
    }

    SECTION("partial application")
    {
        auto all_are_true = hal::all_of([](auto x) { return x; });

        CHECK(!all_are_true(true, false, true));
        CHECK(all_are_true(true));
        CHECK(all_are_true(true, true, true, true, true, true));

        auto all_are_gt_42 = hal::all_of([](auto x) { return x > 42; });

        CHECK(all_are_gt_42(54.3, 100.23, 'A'));
        CHECK(!all_are_gt_42(54.3, -1, 'A'));
    }

    SECTION("reverse")
    {
        auto ss = std::stringstream{};
        CHECK(!hal::reverse::all_of(
            [&](auto x) {
                ss << x;
                return x > 5;
            },
            1, 4, 6, 'a'));
        CHECK(ss.str() == "a64");

        CHECK(hal::reverse::all_of([](auto x) { return x; }, true, true, true));
        CHECK(
            !hal::reverse::all_of([](auto x) { return x; }, true, true, false));
        CHECK(!hal::reverse::all_of([](auto x) { return x; }, false, false));
        CHECK(hal::reverse::all_of([](auto x) { return x; }, true));
        CHECK(!hal::reverse::all_of([](auto x) { return x; }, false));
    }

    SECTION("reverse and partial application")
    {
        auto ss               = std::stringstream{};
        auto all_are_gt_5_rev = hal::reverse::all_of([&](auto x) {
            ss << x;
            return x > 5;
        });

        CHECK(!all_are_gt_5_rev(1, 4, 6, 'a', 100.5));
        CHECK(ss.str() == "100.5a64");

        auto all_are_even_rev =
            hal::reverse::all_of([](auto x) { return x % 2 == 0; });

        CHECK(!all_are_even_rev(2, 4, 12, true, 2));
        CHECK(all_are_even_rev(2, 4, 12, false, 2));
        CHECK(all_are_even_rev(2));
    }

    SECTION("identity default")
    {
        CHECK(!hal::all(true, false, true, true, true));
        CHECK(hal::all(true));
        CHECK(hal::all(true, true, true));
        CHECK(!hal::all(true, (int*)nullptr, true));
        CHECK(!hal::reverse::all(true, false, true, true, true));
        CHECK(hal::reverse::all(true));
        CHECK(hal::reverse::all(true, true, true));
        CHECK(!hal::reverse::all(true, (int*)nullptr, true));
    }

    SECTION("constexpr")
    {
        static_assert(hal::all(true, true, true));
        static_assert(hal::all_of([](auto x) { return x > 42; }, 500, 300.54));
        constexpr auto all_lt_42 = hal::all_of([](auto x) { return x < 42; });
        static_assert(!all_lt_42('a', 4420));

        static_assert(hal::reverse::all(true, true, true));
        static_assert(
            hal::reverse::all_of([](auto x) { return x > 42; }, 500, 300.54));
        constexpr auto all_lt_42_rev =
            hal::reverse::all_of([](auto x) { return x < 42; });
        static_assert(!all_lt_42_rev('a', 4420));
    }
}

TEST_CASE("any_of", "[HAL]")
{
    SECTION("full call")
    {
        CHECK(hal::any_of([](auto x) { return x; }, true, false, true));
        CHECK(hal::any_of([](auto x) { return x; }, true));
        CHECK(!hal::any_of([](auto x) { return x; }, false, false, false));
        CHECK(hal::any_of([](auto x) { return x > 42; }, 54.3, 100.23, 'A'));
        CHECK(hal::any_of([](auto x) { return x > 42; }, 54.3, -1, 'A'));
        CHECK(!hal::any_of([](auto x) { return x > 42; }, 0, 1.4, 3));
    }

    SECTION("partial application")
    {
        auto any_are_true = hal::any_of([](auto x) { return x; });

        CHECK(any_are_true(true, false, true));
        CHECK(any_are_true(true));
        CHECK(!any_are_true(false, false, false, false, false, false));

        auto any_are_gt_42 = hal::any_of([](auto x) { return x > 42; });

        CHECK(any_are_gt_42(54.3, 100.23, 'A'));
        CHECK(any_are_gt_42(54.3, 0.4, 'A'));
        CHECK(!any_are_gt_42(4.3, -1, 5.f));
    }

    SECTION("reverse")
    {
        auto ss = std::stringstream{};
        CHECK(hal::reverse::any_of(
            [&](auto x) {
                ss << x;
                return x > 5;
            },
            1, 4, 6, 'a', 0, -5.4, 2));
        CHECK(ss.str() == "2-5.40a");

        CHECK(hal::reverse::any_of([](auto x) { return x; }, true, true, true));
        CHECK(!hal::reverse::any_of([](auto x) { return x; }, false, false,
                                    false));
        CHECK(!hal::reverse::any_of([](auto x) { return x; }, false, false));
        CHECK(hal::reverse::any_of([](auto x) { return x; }, true));
        CHECK(!hal::reverse::any_of([](auto x) { return x; }, false));
    }

    SECTION("reverse and partial application")
    {
        auto ss               = std::stringstream{};
        auto any_are_gt_5_rev = hal::reverse::any_of([&](auto x) {
            ss << x;
            return x > 5;
        });

        CHECK(any_are_gt_5_rev(1, 4, 6, 'a', 100.5, 0, -2.4, 2));
        CHECK(ss.str() == "2-2.40100.5");

        auto any_are_even_rev =
            hal::reverse::any_of([](auto x) { return x % 2 == 0; });

        CHECK(any_are_even_rev(2, 3, 12, true, 2));
        CHECK(any_are_even_rev(3, 7, 1, true, 2));
        CHECK(!any_are_even_rev(3, 7, 1, true));
        CHECK(!any_are_even_rev()()()()()()()()(1));
    }

    SECTION("identity default")
    {
        CHECK(hal::any(true, false, true, true, true));
        CHECK(!hal::any(false));
        CHECK(hal::any(true));
        CHECK(hal::any(true, true, true));
        CHECK(!hal::any(false, (char*)nullptr, false));
        CHECK(hal::reverse::any(true, false, true, true, true));
        CHECK(!hal::reverse::any(false));
        CHECK(hal::reverse::any(true));
        CHECK(hal::reverse::any(true, true, true));
        CHECK(!hal::reverse::any(false, (double*)nullptr, false));
    }

    SECTION("constexpr")
    {
        static_assert(hal::any(true, (int*)nullptr));
        static_assert(hal::any_of([](auto x) { return x > 42; }, 500, 30.54));
        constexpr auto any_lt_42 = hal::any_of([](auto x) { return x < 42; });
        static_assert(!any_lt_42('a', 4420));

        static_assert(hal::reverse::any(true, (unsigned*)nullptr));
        static_assert(
            hal::reverse::any_of([](auto x) { return x > 42; }, 500, 30.54));
        constexpr auto any_lt_42_rev =
            hal::reverse::any_of([](auto x) { return x < 42; });
        static_assert(!any_lt_42_rev('a', 4420));
    }
}

TEST_CASE("none_of", "[HAL]")
{
    SECTION("full call")
    {
        CHECK(!hal::none_of([](auto x) { return x; }, true, false, true));
        CHECK(!hal::none_of([](auto x) { return x; }, true));
        CHECK(hal::none_of([](auto x) { return x; }, false, false, false));
        CHECK(!hal::none_of([](auto x) { return x > 42; }, 54.3, 100.23, 'A'));
        CHECK(!hal::none_of([](auto x) { return x > 42; }, 54.3, -1, 'A'));
        CHECK(hal::none_of([](auto x) { return x > 42; }, 0, 1.4, 3));
    }

    SECTION("partial application")
    {
        auto none_are_true = hal::none_of([](auto x) { return x; });

        CHECK(!none_are_true(true, false, true));
        CHECK(!none_are_true(true));
        CHECK(none_are_true(false, false, false, false, false, false));

        auto none_are_gt_42 = hal::none_of([](auto x) { return x > 42; });

        CHECK(!none_are_gt_42(54.3, 100.23, 'A'));
        CHECK(!none_are_gt_42(54.3, 0.4, 'A'));
        CHECK(none_are_gt_42(4.3, -1, 5.f));
    }

    SECTION("reverse")
    {
        auto ss = std::stringstream{};
        CHECK(!hal::reverse::none_of(
            [&](auto x) {
                ss << x;
                return x > 5;
            },
            1, 4, 6, 'a', 0, -5.4, 2));
        CHECK(ss.str() == "2-5.40a");

        CHECK(
            !hal::reverse::none_of([](auto x) { return x; }, true, true, true));
        CHECK(hal::reverse::none_of([](auto x) { return x; }, false, false,
                                    false));
        CHECK(hal::reverse::none_of([](auto x) { return x; }, false, false));
        CHECK(!hal::reverse::none_of([](auto x) { return x; }, true));
        CHECK(hal::reverse::none_of([](auto x) { return x; }, false));
    }

    SECTION("reverse and partial application")
    {
        auto ss                = std::stringstream{};
        auto none_are_gt_5_rev = hal::reverse::none_of([&](auto x) {
            ss << x;
            return x > 5;
        });

        CHECK(!none_are_gt_5_rev(1, 4, 6, 'a', 100.5, 0, -2.4, 2));
        CHECK(ss.str() == "2-2.40100.5");

        auto none_are_even_rev =
            hal::reverse::none_of([](auto x) { return x % 2 == 0; });

        CHECK(!none_are_even_rev(2, 3, 12, true, 2));
        CHECK(!none_are_even_rev(3, 7, 1, true, 2));
        CHECK(none_are_even_rev(3, 7, 1, true));
        CHECK(none_are_even_rev(1));
    }

    SECTION("identity default")
    {
        CHECK(!hal::none(true, false, true, true, true));
        CHECK(hal::none(false));
        CHECK(!hal::none(true));
        CHECK(!hal::none(true, true, true));
        CHECK(hal::none(false, (int*)nullptr, false));
        CHECK(!hal::reverse::none(true, false, true, true, true));
        CHECK(hal::reverse::none(false));
        CHECK(!hal::reverse::none(true));
        CHECK(!hal::reverse::none(true, true, true));
        CHECK(hal::reverse::none(false, (int*)nullptr, false));
    }

    SECTION("constexpr")
    {
        static_assert(hal::none(false, (char*)nullptr));
        static_assert(hal::none_of([](auto x) { return x > 42; }, 2, 30.54));
        constexpr auto none_lt_42 = hal::none_of([](auto x) { return x < 42; });
        static_assert(none_lt_42('a', 4420));

        static_assert(hal::reverse::none(false, (bool*)nullptr));
        static_assert(
            hal::reverse::none_of([](auto x) { return x > 42; }, 2, 30.54));
        constexpr auto none_lt_42_rev =
            hal::reverse::none_of([](auto x) { return x < 42; });
        static_assert(none_lt_42_rev('a', 4420));
    }
}
