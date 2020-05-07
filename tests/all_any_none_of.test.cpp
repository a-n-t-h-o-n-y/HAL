#include <sstream>

#include <catch2/catch.hpp>

#include <halg.hpp>

TEST_CASE("all_of", "[HALG]")
{
    SECTION("unbound")
    {
        CHECK(!halg::all_of([](auto x) { return x; }, true, false, true));
        CHECK(halg::all_of([](auto x) { return x; }, true));
        CHECK(halg::all_of([](auto x) { return x > 42; }, 54.3, 100.23, 'A'));
        CHECK(!halg::all_of([](auto x) { return x > 42; }, 54.3, -1, 'A'));
    }

    SECTION("bound")
    {
        auto all_are_true = halg::all_of([](auto x) { return x; });

        CHECK(!all_are_true(true, false, true));
        CHECK(all_are_true(true));
        CHECK(all_are_true(true, true, true, true, true, true));

        auto all_are_gt_42 = halg::all_of([](auto x) { return x > 42; });

        CHECK(all_are_gt_42(54.3, 100.23, 'A'));
        CHECK(!all_are_gt_42(54.3, -1, 'A'));
    }

    SECTION("reverse")
    {
        auto ss = std::stringstream{};
        CHECK(!halg::reverse::all_of(
            [&](auto x) {
                ss << x;
                return x > 5;
            },
            1, 4, 6, 'a'));
        CHECK(ss.str() == "a64");

        CHECK(
            halg::reverse::all_of([](auto x) { return x; }, true, true, true));
        CHECK(!halg::reverse::all_of([](auto x) { return x; }, true, true,
                                     false));
        CHECK(!halg::reverse::all_of([](auto x) { return x; }, false, false));
        CHECK(halg::reverse::all_of([](auto x) { return x; }, true));
        CHECK(!halg::reverse::all_of([](auto x) { return x; }, false));
    }

    SECTION("reverse and bound")
    {
        auto ss               = std::stringstream{};
        auto all_are_gt_5_rev = halg::reverse::all_of([&](auto x) {
            ss << x;
            return x > 5;
        });

        CHECK(!all_are_gt_5_rev(1, 4, 6, 'a', 100.5));
        CHECK(ss.str() == "100.5a64");

        auto all_are_even_rev =
            halg::reverse::all_of([](auto x) { return x % 2 == 0; });

        CHECK(!all_are_even_rev(2, 4, 12, true, 2));
        CHECK(all_are_even_rev(2, 4, 12, false, 2));
        CHECK(all_are_even_rev(2));
    }
}

TEST_CASE("any_of", "[HALG]")
{
    SECTION("unbound")
    {
        CHECK(halg::any_of([](auto x) { return x; }, true, false, true));
        CHECK(halg::any_of([](auto x) { return x; }, true));
        CHECK(!halg::any_of([](auto x) { return x; }, false, false, false));
        CHECK(halg::any_of([](auto x) { return x > 42; }, 54.3, 100.23, 'A'));
        CHECK(halg::any_of([](auto x) { return x > 42; }, 54.3, -1, 'A'));
        CHECK(!halg::any_of([](auto x) { return x > 42; }, 0, 1.4, 3));
    }

    SECTION("bound")
    {
        auto any_are_true = halg::any_of([](auto x) { return x; });

        CHECK(any_are_true(true, false, true));
        CHECK(any_are_true(true));
        CHECK(!any_are_true(false, false, false, false, false, false));

        auto any_are_gt_42 = halg::any_of([](auto x) { return x > 42; });

        CHECK(any_are_gt_42(54.3, 100.23, 'A'));
        CHECK(any_are_gt_42(54.3, 0.4, 'A'));
        CHECK(!any_are_gt_42(4.3, -1, 5.f));
    }

    SECTION("reverse")
    {
        auto ss = std::stringstream{};
        CHECK(halg::reverse::any_of(
            [&](auto x) {
                ss << x;
                return x > 5;
            },
            1, 4, 6, 'a', 0, -5.4, 2));
        CHECK(ss.str() == "2-5.40a");

        CHECK(
            halg::reverse::any_of([](auto x) { return x; }, true, true, true));
        CHECK(!halg::reverse::any_of([](auto x) { return x; }, false, false,
                                     false));
        CHECK(!halg::reverse::any_of([](auto x) { return x; }, false, false));
        CHECK(halg::reverse::any_of([](auto x) { return x; }, true));
        CHECK(!halg::reverse::any_of([](auto x) { return x; }, false));
    }

    SECTION("reverse and bound")
    {
        auto ss               = std::stringstream{};
        auto any_are_gt_5_rev = halg::reverse::any_of([&](auto x) {
            ss << x;
            return x > 5;
        });

        CHECK(any_are_gt_5_rev(1, 4, 6, 'a', 100.5, 0, -2.4, 2));
        CHECK(ss.str() == "2-2.40100.5");

        auto any_are_even_rev =
            halg::reverse::any_of([](auto x) { return x % 2 == 0; });

        CHECK(any_are_even_rev(2, 3, 12, true, 2));
        CHECK(any_are_even_rev(3, 7, 1, true, 2));
        CHECK(!any_are_even_rev(3, 7, 1, true));
        CHECK(!any_are_even_rev()()()()()()()()(1));
    }
}

TEST_CASE("none_of", "[HALG]")
{
    SECTION("unbound")
    {
        CHECK(!halg::none_of([](auto x) { return x; }, true, false, true));
        CHECK(!halg::none_of([](auto x) { return x; }, true));
        CHECK(halg::none_of([](auto x) { return x; }, false, false, false));
        CHECK(!halg::none_of([](auto x) { return x > 42; }, 54.3, 100.23, 'A'));
        CHECK(!halg::none_of([](auto x) { return x > 42; }, 54.3, -1, 'A'));
        CHECK(halg::none_of([](auto x) { return x > 42; }, 0, 1.4, 3));
    }

    SECTION("bound")
    {
        auto none_are_true = halg::none_of([](auto x) { return x; });

        CHECK(!none_are_true(true, false, true));
        CHECK(!none_are_true(true));
        CHECK(none_are_true(false, false, false, false, false, false));

        auto none_are_gt_42 = halg::none_of([](auto x) { return x > 42; });

        CHECK(!none_are_gt_42(54.3, 100.23, 'A'));
        CHECK(!none_are_gt_42(54.3, 0.4, 'A'));
        CHECK(none_are_gt_42(4.3, -1, 5.f));
    }

    SECTION("reverse")
    {
        auto ss = std::stringstream{};
        CHECK(!halg::reverse::none_of(
            [&](auto x) {
                ss << x;
                return x > 5;
            },
            1, 4, 6, 'a', 0, -5.4, 2));
        CHECK(ss.str() == "2-5.40a");

        CHECK(!halg::reverse::none_of([](auto x) { return x; }, true, true,
                                      true));
        CHECK(halg::reverse::none_of([](auto x) { return x; }, false, false,
                                     false));
        CHECK(halg::reverse::none_of([](auto x) { return x; }, false, false));
        CHECK(!halg::reverse::none_of([](auto x) { return x; }, true));
        CHECK(halg::reverse::none_of([](auto x) { return x; }, false));
    }

    SECTION("reverse and bound")
    {
        auto ss                = std::stringstream{};
        auto none_are_gt_5_rev = halg::reverse::none_of([&](auto x) {
            ss << x;
            return x > 5;
        });

        CHECK(!none_are_gt_5_rev(1, 4, 6, 'a', 100.5, 0, -2.4, 2));
        CHECK(ss.str() == "2-2.40100.5");

        auto none_are_even_rev =
            halg::reverse::none_of([](auto x) { return x % 2 == 0; });

        CHECK(!none_are_even_rev(2, 3, 12, true, 2));
        CHECK(!none_are_even_rev(3, 7, 1, true, 2));
        CHECK(none_are_even_rev(3, 7, 1, true));
        CHECK(none_are_even_rev(1));
    }
}
