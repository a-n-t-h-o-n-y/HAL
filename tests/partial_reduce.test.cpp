#include <tuple>

#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("partial_reduce", "[HAL]")
{
    SECTION("empty parameter pack")
    {
        [](auto... xs) { hal::partial_reduce(0, std::plus<>{}, xs...); }();
    }
    SECTION("full call")
    {
        [](auto... xs) {
            hal::partial_reduce(0, std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);
    }
    SECTION("partial application")
    {
        auto init_pr = hal::partial_reduce(0);
        [&](auto... xs) {
            init_pr(std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);

        auto ps = hal::partial_reduce(0, std::plus<>{});
        [&](auto... xs) {
            ps(xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);
    }
    SECTION("partial_sum")
    {
        [&](auto... xs) {
            hal::partial_sum(xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);
    }
    SECTION("partial_difference")
    {
        [&](auto... xs) {
            hal::partial_difference(xs...);
            CHECK(hal::get<0>(xs...) == -1);
            CHECK(hal::get<1>(xs...) == -3);
            CHECK(hal::get<2>(xs...) == -6);
            CHECK(hal::get<3>(xs...) == -2);
            CHECK(hal::get<4>(xs...) == -5.2);
            CHECK(hal::get<5>(xs...) == -7);
        }(1, 2, 3, -4, 3.2, 2);
    }
    SECTION("partial_product")
    {
        [&](auto... xs) {
            hal::partial_product(xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 2);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == -24);
            CHECK(hal::get<4>(xs...) == -24 * 3.2);
            CHECK(hal::get<5>(xs...) == -152);
        }(1, 2, 3, -4, 3.2, 2);
    }
    SECTION("partial_quotient")
    {
        [&](auto... xs) {
            hal::partial_quotient(xs...);
            CHECK(hal::get<0>(xs...) == 1.);
            CHECK(hal::get<1>(xs...) == .5);
            CHECK(hal::get<2>(xs...) == .5 / 3.);
            CHECK(hal::get<3>(xs...) == (.5 / 3.) / -4.);
            CHECK(hal::get<4>(xs...) == ((.5 / 3.) / -4.) / 0.000002);
            CHECK(hal::get<5>(xs...) == -694);
        }(1., 2., 3., -4., 0.000002, 30);
    }
}
