#include <tuple>

#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("partial_transform_reduce", "[HAL]")
{
    SECTION("empty parameter pack")
    {
        [](auto... xs) {
            hal::partial_transform_reduce(0, hal::detail::Identity{},
                                          std::plus<>{}, xs...);
        }();
    }
    SECTION("full call")
    {
        [](auto... xs) {
            hal::partial_transform_reduce(0., hal::detail::Identity{},
                                          std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);

        [](auto... xs) {
            hal::partial_transform_reduce(
                0, [](auto x) { return x % 2; }, std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 1);
            CHECK(hal::get<2>(xs...) == 2);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 3);
            CHECK(hal::get<5>(xs...) == 3);
        }(1, 2, 3, -4, 3, 2);
    }
    SECTION("partial application")
    {
        auto init_ptr = hal::partial_transform_reduce(0.);
        [&](auto... xs) {
            init_ptr(hal::detail::Identity{}, std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);

        auto t_even =
            hal::partial_transform_reduce(0, [](auto x) { return x % 2; });
        [&](auto... xs) {
            t_even(std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 1);
            CHECK(hal::get<2>(xs...) == 2);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 3);
            CHECK(hal::get<5>(xs...) == 3);
        }(1, 2, 3, -4, 3, 2);

        auto t_even_sum = hal::partial_transform_reduce(
            0, [](auto x) { return x % 2; }, std::plus<>{});
        [&](auto... xs) {
            t_even_sum(xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 1);
            CHECK(hal::get<2>(xs...) == 2);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 3);
            CHECK(hal::get<5>(xs...) == 3);
        }(1, 2, 3, -4, 3, 2);
    }
    SECTION("reverse::partial_transform_reduce")
    {
        SECTION("full call")
        {
            [](auto... xs) {
                hal::reverse::partial_transform_reduce(
                    0., hal::detail::Identity{}, std::plus<>{}, xs...);
                CHECK(hal::get<0>(xs...) == 7);
                CHECK(hal::get<1>(xs...) == 6);
                CHECK(hal::get<2>(xs...) == 4);
                CHECK(hal::get<3>(xs...) == 1);
                CHECK(hal::get<4>(xs...) == 5.2);
                CHECK(hal::get<5>(xs...) == 2);
            }(1, 2, 3, -4, 3.2, 2);

            [](auto... xs) {
                hal::reverse::partial_transform_reduce(
                    0, [](auto x) { return x % 2; }, std::plus<>{}, xs...);
                CHECK(hal::get<0>(xs...) == 3);
                CHECK(hal::get<1>(xs...) == 2);
                CHECK(hal::get<2>(xs...) == 2);
                CHECK(hal::get<3>(xs...) == 1);
                CHECK(hal::get<4>(xs...) == 1);
                CHECK(hal::get<5>(xs...) == 0);
            }(1, 2, 3, -4, 3, 2);
        }
        SECTION("partial application")
        {
            auto init_ptr = hal::reverse::partial_transform_reduce(0.);
            [&](auto... xs) {
                init_ptr(hal::detail::Identity{}, std::plus<>{}, xs...);
                CHECK(hal::get<0>(xs...) == 7);
                CHECK(hal::get<1>(xs...) == 6);
                CHECK(hal::get<2>(xs...) == 4);
                CHECK(hal::get<3>(xs...) == 1);
                CHECK(hal::get<4>(xs...) == 5.2);
                CHECK(hal::get<5>(xs...) == 2);
            }(1, 2, 3, -4, 3.2, 2);

            auto t_even = hal::reverse::partial_transform_reduce(
                0, [](auto x) { return x % 2; });
            [&](auto... xs) {
                t_even(std::plus<>{}, xs...);
                CHECK(hal::get<0>(xs...) == 3);
                CHECK(hal::get<1>(xs...) == 2);
                CHECK(hal::get<2>(xs...) == 2);
                CHECK(hal::get<3>(xs...) == 1);
                CHECK(hal::get<4>(xs...) == 1);
                CHECK(hal::get<5>(xs...) == 0);
            }(1, 2, 3, -4, 3, 2);

            auto t_even_sum = hal::reverse::partial_transform_reduce(
                0, [](auto x) { return x % 2; }, std::plus<>{});
            [&](auto... xs) {
                t_even_sum(xs...);
                CHECK(hal::get<0>(xs...) == 3);
                CHECK(hal::get<1>(xs...) == 2);
                CHECK(hal::get<2>(xs...) == 2);
                CHECK(hal::get<3>(xs...) == 1);
                CHECK(hal::get<4>(xs...) == 1);
                CHECK(hal::get<5>(xs...) == 0);
            }(1, 2, 3, -4, 3, 2);
        }
    }
}
