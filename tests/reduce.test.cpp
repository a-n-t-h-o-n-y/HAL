#include <sstream>

#include <catch2/catch.hpp>

#include <hal.hpp>

namespace {
constexpr auto sum         = [](auto x, auto y) { return x + y; };
constexpr auto product     = [](auto x, auto y) { return x * y; };
constexpr auto logical_and = [](auto x, auto y) { return x && y; };
constexpr auto logical_or  = [](auto x, auto y) { return x || y; };

struct Foo {
    int a    = 3;
    char b   = '#';
    double c = 5.212;
};

struct Empty {};
}  // namespace

TEST_CASE("hal::reduce", "[HAL]")
{
    SECTION("call order")
    {
        auto s = hal::reduce(
            std::string{},
            [](auto const& sum, auto const& x) {
                auto ss = std::stringstream{};
                ss << x;
                return sum + ss.str();
            },
            1, 2, 3, 'a');
        CHECK(s == "123a");
    }

    SECTION("full call")
    {
        auto const a = hal::reduce(0., sum, 1, 'a', .5);
        CHECK(a == 0. + 1 + 'a' + .5);
        auto const b = hal::reduce(true, logical_and, 1, true, (bool)nullptr);
        CHECK(!b);
        auto const n = hal::reduce(true, logical_and, (bool)nullptr);
        CHECK(!n);
        auto const c = hal::reduce(false, logical_or, 1, true, (bool)nullptr);
        CHECK(c);
        auto const d = hal::reduce(false, logical_or, true, true, true);
        CHECK(d);
        auto const e = hal::reduce(false, logical_or, true);
        CHECK(e);
        auto const f = hal::reduce(true, logical_and, true);
        CHECK(f);
    }

    SECTION("partial application")
    {
        auto empty_reduce = hal::reduce(0);
        auto sum_reduce   = empty_reduce(sum);
        CHECK(sum_reduce(1, 2, 3, 4) == 10);
        CHECK(sum_reduce(1) == 1);
        CHECK(sum_reduce() == 0);

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

TEST_CASE("hal::reverse::reduce", "[HAL]")
{
    namespace hr = hal::reverse;
    SECTION("call order")
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
}

TEST_CASE("hal::memberwise::reduce", "[HAL]")
{
    SECTION("call order")
    {
        auto s = hal::memberwise::reduce(
            std::string{},
            [](auto const& sum, auto const& x) {
                auto ss = std::stringstream{};
                ss << x;
                return sum + ss.str();
            },
            Foo{});
        CHECK(s == "3#5.212");
    }

    SECTION("full call")
    {
        auto const a = hal::memberwise::reduce(0., sum, Foo{});
        CHECK(a == (3 + '#' + 5.212));
        auto const f = Foo{};
        auto const b = hal::memberwise::reduce(0, sum, f);
        CHECK(b == (3 + '#' + 5));
        auto const c = hal::memberwise::reduce(0, sum, Empty{});
        CHECK(c == 0);
    }

    SECTION("partial application")
    {
        auto empty_reduce = hal::memberwise::reduce(0);
        auto sum_reduce   = empty_reduce(sum);
        auto const f      = Foo{};

        CHECK(empty_reduce(sum, f) == (3 + '#' + 5));
        CHECK(empty_reduce(sum, Empty{}) == 0);

        CHECK(sum_reduce(Foo{}) == (3 + '#' + 5));
        CHECK(sum_reduce(f) == (3 + '#' + 5));
        CHECK(sum_reduce(Empty{}) == 0);

        auto product_reduce = hal::memberwise::reduce(1., product);
        CHECK(product_reduce(f) == (3 * '#' * 5.212));
        CHECK(product_reduce(Foo{}) == (3 * '#' * 5.212));
    }

    SECTION("constexpr")
    {
        constexpr auto empty_reduce = hal::memberwise::reduce(0.);
        constexpr auto unary_reduce = hal::memberwise::reduce(1.);
        constexpr auto sum_reduce   = empty_reduce(sum);
        static_assert(unary_reduce(product, Foo{}) == (3 * '#' * 5.212));
        static_assert(unary_reduce(product, Empty{}) == 1.);

        constexpr auto f = Foo{};
        static_assert(sum_reduce(Foo{}) == (3 + '#' + 5.212));
        static_assert(sum_reduce(f) == (3 + '#' + 5.212));
        static_assert(sum_reduce(Empty{}) == 0.);

        constexpr auto product_reduce = hal::memberwise::reduce(1., product);
        static_assert(product_reduce(f) == (3 * '#' * 5.212));
        static_assert(product_reduce(Foo{}) == (3 * '#' * 5.212));
        static_assert(product_reduce(Empty{}) == 1.);
    }
}

TEST_CASE("hal::reverse::memberwise::reduce", "[HAL]")
{
    SECTION("call order")
    {
        auto s = hal::reverse::memberwise::reduce(
            std::string{},
            [](auto const& sum, auto const& x) {
                auto ss = std::stringstream{};
                ss << x;
                return sum + ss.str();
            },
            Foo{});
        CHECK(s == "5.212#3");
    }

    SECTION("full call")
    {
        auto const f = Foo{};
        auto const b = hal::reverse::memberwise::reduce(0, sum, f);
        CHECK(b == (5 + '#' + 3));
        auto const c = hal::reverse::memberwise::reduce(0, sum, Empty{});
        CHECK(c == 0);
    }

    SECTION("partial application")
    {
        auto empty_reduce = hal::reverse::memberwise::reduce(0);
        auto sum_reduce   = empty_reduce(sum);
        auto const f      = Foo{};

        CHECK(empty_reduce(sum, f) == 3 + '#' + 5);
        CHECK(empty_reduce(sum, Empty{}) == 0);

        CHECK(sum_reduce(Foo{}) == 3 + '#' + 5);
        CHECK(sum_reduce(f) == 3 + '#' + 5);
        CHECK(sum_reduce(Empty{}) == 0);

        auto product_reduce = hal::reverse::memberwise::reduce(1., product);
        CHECK(product_reduce(f) == 3 * '#' * 5.212);
        CHECK(product_reduce(Foo{}) == 3 * '#' * 5.212);
    }

    SECTION("constexpr")
    {
        constexpr auto empty_reduce = hal::reverse::memberwise::reduce(0.);
        constexpr auto unary_reduce = hal::reverse::memberwise::reduce(1.);
        constexpr auto sum_reduce   = empty_reduce(sum);
        static_assert(unary_reduce(product, Foo{}) == (3 * '#' * 5.212));
        static_assert(unary_reduce(product, Empty{}) == 1.);

        constexpr auto f = Foo{};
        static_assert(sum_reduce(Foo{}) == (3 + '#' + 5.212));
        static_assert(sum_reduce(f) == (3 + '#' + 5.212));
        static_assert(sum_reduce(Empty{}) == 0.);

        constexpr auto product_reduce =
            hal::reverse::memberwise::reduce(1., product);
        static_assert(product_reduce(f) == (3 * '#' * 5.212));
        static_assert(product_reduce(Foo{}) == (3 * '#' * 5.212));
        static_assert(product_reduce(Empty{}) == 1.);
    }
}
