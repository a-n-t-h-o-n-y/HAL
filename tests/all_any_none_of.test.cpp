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
}
