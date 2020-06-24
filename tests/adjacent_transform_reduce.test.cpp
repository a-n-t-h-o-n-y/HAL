#include <cctype>
#include <functional>
#include <sstream>

#include <catch2/catch.hpp>

#include <hal.hpp>

constexpr auto product = [](auto x, auto y) { return x * y; };
constexpr auto sum     = [](auto x, auto y) { return x + y; };
TEST_CASE("adjacent_tranform_reduce", "[HAL]")
{
    SECTION("full call")
    {
        using namespace hal;
        CHECK(adjacent_transform_reduce(0, product, sum, 1, 2) == 2);
        CHECK(adjacent_transform_reduce(0, product, sum, 1, 2, 3) == 8);
        CHECK(adjacent_transform_reduce(0, product, sum, 1, 2, 3, 5) == 23);

        CHECK(adjacent_transform_reduce(0, sum, sum, 1, 2, 3, 5) == 16);

        auto is_same_case = [](auto a, auto b) {
            return std::isupper(a) == std::isupper(b);
        };
        // are_all_same_case
        CHECK(adjacent_transform_reduce(true, is_same_case,
                                        std::logical_and<>{}, 'a', 'b', 'c'));
        CHECK(!adjacent_transform_reduce(true, is_same_case,
                                         std::logical_and<>{}, 'a', 'B', 'C'));
    }
    SECTION("partial application")
    {
        auto apply_1 = hal::adjacent_transform_reduce(0);
        CHECK(apply_1(product, sum, 1, 2, 3, 5) == 23);

        auto apply_2 = hal::adjacent_transform_reduce(0, product);
        CHECK(apply_2(sum, 1, 2, 3, 5) == 23);

        auto apply_3 = hal::adjacent_transform_reduce(0, product, sum);
        CHECK(apply_3(1, 2, 3, 5) == 23);
    }
    SECTION("constexpr")
    {
        using namespace hal;
        static_assert(adjacent_transform_reduce(0, product, sum, 1, 2) == 2);
        static_assert(adjacent_transform_reduce(0, product, sum, 1, 2, 3) == 8);
        static_assert(adjacent_transform_reduce(0, sum, sum, 1, 2, 3, 5) == 16);

        constexpr auto apply_1 = hal::adjacent_transform_reduce(0);
        static_assert(apply_1(product, sum, 1, 2, 3, 5) == 23);

        constexpr auto apply_2 = hal::adjacent_transform_reduce(0, product);
        static_assert(apply_2(sum, 1, 2, 3, 5) == 23);

        constexpr auto apply_3 =
            hal::adjacent_transform_reduce(0, product, sum);
        static_assert(apply_3(1, 2, 3, 5) == 23);
    }
}
