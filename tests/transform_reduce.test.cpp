#include <sstream>

#include <catch2/catch.hpp>

#include <hal.hpp>

constexpr auto square = [](auto x) { return x * x; };
constexpr auto sum    = [](auto x, auto y) { return x + y; };
TEST_CASE("tranform_reduce", "[HAL]")
{
    SECTION("full call")
    {
        auto const a = hal::transform_reduce(0, square, sum, 1, 2, 3, 4, 5);
        CHECK(a == 55);

        auto const b = hal::transform_reduce(0, square, sum, 5.4, 'a');
        CHECK(b == 9438);
    }
    SECTION("partial application")
    {
        auto is_two     = [](auto x) { return x == 2; };
        auto inc_if_two = [&](auto count, auto x) {
            return is_two(x) ? count + 1 : count;
        };

        // Apply One
        auto init_tr = hal::transform_reduce(0);
        // ...apply one more
        auto empty_reduce_0 = init_tr(hal::detail::Identity{});
        // ......apply one more
        auto count_twos_0 = empty_reduce_0(inc_if_two);
        CHECK(count_twos_0(1, 4, 2, 2, 5, 2) == 3);

        // ...apply two more
        auto count_twos_1 = init_tr(hal::detail::Identity{}, inc_if_two);
        CHECK(count_twos_1(1, 4, 2, 2, 5, 2) == 3);

        // ...full application
        auto twos_count_2 =
            init_tr(hal::detail::Identity{}, inc_if_two, 1, 4, 2, 2, 5, 2);
        CHECK(twos_count_2 == 3);

        // Apply Two
        auto empty_reduce = hal::transform_reduce(0, hal::detail::Identity{});
        auto count_twos_2 = empty_reduce(inc_if_two);
        CHECK(count_twos_2(1, 4, 2, 2, 5, 2) == 3);
        CHECK(empty_reduce(inc_if_two, 1, 4, 2, 2, 5, 2) == 3);

        // Apply Three
        auto count_twos_3 =
            hal::transform_reduce(0, hal::detail::Identity{}, inc_if_two);
        auto twos = count_twos_3(1, 1, 2, 2.0, 4, 2, 'a', 32.1);
        CHECK(twos == 3);
    }
    SECTION("constexpr")
    {
        constexpr auto is_two     = [](auto x) { return x == 2; };
        constexpr auto inc_if_two = [=](auto count, auto x) {
            return is_two(x) ? count + 1 : count;
        };

        // Apply One
        {
            constexpr auto init_tr      = hal::transform_reduce(0);
            constexpr auto empty_reduce = init_tr(hal::detail::Identity{});
            constexpr auto count_twos   = empty_reduce(inc_if_two);
            static_assert(count_twos(1, 4, 2, 2, 5, 2, 'a') == 3);
            static_assert(count_twos('a') == 0);
        }

        // Apply Two
        {
            constexpr auto empty_reduce =
                hal::transform_reduce(0, hal::detail::Identity{});
            constexpr auto count_twos = empty_reduce(inc_if_two);
            static_assert(count_twos(1, 4, 'a', 2, 5, 2) == 2);
        }

        // Apply Three
        {
            constexpr auto count_twos =
                hal::transform_reduce(0, hal::detail::Identity{}, inc_if_two);
            static_assert(count_twos(1, 1, 2, 2.0, 4, 2, 'a', 32.1) == 3);
        }
    }
}

TEST_CASE("reverse::tranform_reduce", "[HAL]")
{
    namespace hr = hal::reverse;
    SECTION("full call")
    {
        auto ss               = std::stringstream{};
        auto square_and_print = [&ss](auto x) {
            ss << x;
            return x * x;
        };
        auto const a =
            hr::transform_reduce(0, square_and_print, sum, 1, 2, 3, 4, 5);
        CHECK(a == 55);
        CHECK(ss.str() == "54321");

        ss.str(std::string{});
        auto const b = hr::transform_reduce(0, square_and_print, sum, 5.4, 'a');
        CHECK(b == 9438);
        CHECK(ss.str() == "a5.4");
    }
    SECTION("partial application")
    {
        auto is_two     = [](auto x) { return x == 2; };
        auto inc_if_two = [&](auto count, auto x) {
            return is_two(x) ? count + 1 : count;
        };

        // Apply One
        auto init_tr = hr::transform_reduce(0);
        // ...apply one more
        auto empty_reduce_0 = init_tr(hal::detail::Identity{});
        // ......apply one more
        auto count_twos_0 = empty_reduce_0(inc_if_two);
        CHECK(count_twos_0(1, 4, 2, 2, 5, 2) == 3);

        // ...apply two more
        auto count_twos_1 = init_tr(hal::detail::Identity{}, inc_if_two);
        CHECK(count_twos_1(1, 4, 2, 2, 5, 2) == 3);

        // ...full application
        auto twos_count_2 =
            init_tr(hal::detail::Identity{}, inc_if_two, 1, 4, 2, 2, 5, 2);
        CHECK(twos_count_2 == 3);

        // Apply Two
        auto empty_reduce = hr::transform_reduce(0, hal::detail::Identity{});
        auto count_twos_2 = empty_reduce(inc_if_two);
        CHECK(count_twos_2(1, 4, 2, 2, 5, 2) == 3);
        CHECK(empty_reduce(inc_if_two, 1, 4, 2, 2, 5, 2) == 3);

        // Apply Three
        auto count_twos_3 =
            hr::transform_reduce(0, hal::detail::Identity{}, inc_if_two);
        auto twos = count_twos_3(1, 1, 2, 2.0, 4, 2, 'a', 32.1);
        CHECK(twos == 3);
    }
    SECTION("constexpr")
    {
        constexpr auto is_two     = [](auto x) { return x == 2; };
        constexpr auto inc_if_two = [=](auto count, auto x) {
            return is_two(x) ? count + 1 : count;
        };

        // Apply One
        {
            constexpr auto init_tr      = hr::transform_reduce(0);
            constexpr auto empty_reduce = init_tr(hal::detail::Identity{});
            constexpr auto count_twos   = empty_reduce(inc_if_two);
            static_assert(count_twos(1, 4, 2, 2, 5, 2, 'a') == 3);
            static_assert(count_twos('a') == 0);
        }

        // Apply Two
        {
            constexpr auto empty_reduce =
                hr::transform_reduce(0, hal::detail::Identity{});
            constexpr auto count_twos = empty_reduce(inc_if_two);
            static_assert(count_twos(1, 4, 'a', 2, 5, 2) == 2);
        }

        // Apply Three
        {
            constexpr auto count_twos =
                hr::transform_reduce(0, hal::detail::Identity{}, inc_if_two);
            static_assert(count_twos(1, 1, 2, 2.0, 4, 2, 'a', 32.1) == 3);
        }
    }
}
