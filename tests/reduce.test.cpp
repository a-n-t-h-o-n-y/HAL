#include <sstream>

#include <catch2/catch.hpp>

#include <hal.hpp>

namespace {
constexpr auto sum         = [](auto x, auto y) { return x + y; };
constexpr auto product     = [](auto x, auto y) { return x * y; };
constexpr auto logical_and = [](auto x, auto y) { return x && y; };
constexpr auto logical_or  = [](auto x, auto y) { return x || y; };
}  // namespace

TEST_CASE("reduce", "[HAL]")
{
    SECTION("full call")
    {
        auto a = hal::reduce(0., sum, 1, 'a', .5);
        CHECK(a == 0. + 1 + 'a' + .5);
        auto b = hal::reduce(true, logical_and, 1, true, (bool)nullptr);
        CHECK(!b);
        auto n = hal::reduce(true, logical_and, (bool)nullptr);
        CHECK(!n);
        auto c = hal::reduce(false, logical_or, 1, true, (bool)nullptr);
        CHECK(c);
        auto d = hal::reduce(false, logical_or, true, true, true);
        CHECK(d);
        auto e = hal::reduce(false, logical_or, true);
        CHECK(e);
        auto f = hal::reduce(true, logical_and, true);
        CHECK(f);
    }
    SECTION("partial application")
    {
        auto empty_reduce = hal::reduce(0);
        auto sum_reduce   = empty_reduce(sum);
        CHECK(sum_reduce(1, 2, 3, 4) == 10);
        CHECK(sum_reduce(1) == 1);

        CHECK(empty_reduce(sum, 1, 2, 3, 4) == 10);

        auto product_reduce = hal::reduce(1, product);
        CHECK(product_reduce(4, 3, 2, 1) == 24);

        auto concat =
            hal::reduce(std::string{}, [](auto x, auto y) { return x + y; });
        CHECK(concat("Hello", ',', ' ', "World!") == "Hello, World!");
    }
    SECTION("constexpr")
    {
        constexpr auto empty_reduce = hal::reduce(0);
        constexpr auto sum_reduce   = empty_reduce(sum);
        static_assert(sum_reduce(1, 2, 3, 4) == 10);
        static_assert(sum_reduce(1) == 1);

        constexpr auto product_reduce = hal::reduce(1, product);
        static_assert(product_reduce(4, 3, 2, 1) == 24);
    }
}

TEST_CASE("reverse::reduce", "[HAL]")
{
    namespace hr = hal::reverse;
    SECTION("call ordering")
    {
        auto ss            = std::stringstream{};
        auto sum_and_print = [&ss](auto x, auto y) {
            ss << x << y;
            return x + y;
        };

        auto const a = hr::reduce(0, sum_and_print, 1, 2, 3);
        CHECK(ss.str() == "033251");
        CHECK(a == 6);

        ss.str(std::string{});
        hr::reduce(0, sum_and_print, 'a', 1);
        CHECK(ss.str() == "011a");
    }
    SECTION("full call")
    {
        auto a = hr::reduce(0., sum, 1, 'a', .5);
        CHECK(a == 0. + 1 + 'a' + .5);
        auto b = hr::reduce(true, logical_and, 1, true, (bool)nullptr);
        CHECK(!b);
        auto n = hr::reduce(true, logical_and, (bool)nullptr);
        CHECK(!n);
        auto c = hr::reduce(false, logical_or, 1, true, (bool)nullptr);
        CHECK(c);
        auto d = hr::reduce(false, logical_or, true, true, true);
        CHECK(d);
        auto e = hr::reduce(false, logical_or, true);
        CHECK(e);
        auto f = hr::reduce(true, logical_and, true);
        CHECK(f);
    }
    SECTION("partial application")
    {
        auto empty_reduce = hr::reduce(0);
        auto sum_reduce   = empty_reduce(sum);
        CHECK(sum_reduce(1, 2, 3, 4) == 10);
        CHECK(sum_reduce(1) == 1);

        auto product_reduce = hr::reduce(1, product);
        CHECK(product_reduce(4, 3, 2, 1) == 24);

        auto concat_rev =
            hr::reduce(std::string{}, [](auto x, auto y) { return x + y; });
        CHECK(concat_rev("Hello", ',', ' ', "World!") == "World! ,Hello");
    }
    SECTION("constexpr")
    {
        constexpr auto empty_reduce = hr::reduce(0);
        constexpr auto sum_reduce   = empty_reduce(sum);
        static_assert(sum_reduce(1, 2, 3, 4) == 10);
        static_assert(sum_reduce(1) == 1);

        constexpr auto product_reduce = hr::reduce(1, product);
        static_assert(product_reduce(4, 3, 2, 1) == 24);
    }
    SECTION("hal::memberwise::reduce")
    {
        // TODO
    }
}
