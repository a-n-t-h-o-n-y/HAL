#include <sstream>
#include <string>

#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("hal::for_each", "[HAL]")
{
    SECTION("full call & call order")
    {
        auto ss = std::stringstream{};

        hal::for_each([&ss](auto const& x) { ss << x; }, 2, 6.3, ' ',
                      "hello, world!", -432, 0.5);

        CHECK(ss.str() == "26.3 hello, world!-4320.5");
    }

    SECTION("modifying")
    {
        [](auto... args) {
            hal::for_each([](auto& x) { x = 1; }, args...);
            hal::for_each([](auto x) { CHECK(x == 1); }, args...);
        }(1, 5, 3);
    }

    SECTION("partial application")
    {
        auto ss          = std::stringstream{};
        auto write_to_ss = hal::for_each([&ss](auto const& x) { ss << x; });

        write_to_ss(2, 6.3, ' ', "hello, world!", -432);
        write_to_ss(0.5);
        write_to_ss();
        CHECK(ss.str() == "26.3 hello, world!-4320.5");
    }

    SECTION("constexpr")
    {
        constexpr auto nop = hal::for_each([](auto) {});
        static_assert((nop('a', 1, 4, 2), true));
        static_assert((hal::for_each([](auto) {}, nullptr, 'a', 4.3), true));
    }
}

TEST_CASE("reverse::for_each", "[HAL]")
{
    SECTION("full call & call order")
    {
        auto ss = std::stringstream{};

        hal::reverse::for_each([&ss](auto const& x) { ss << x; }, 2, 6.3, ' ',
                               "hello, world!", -432, 0.5);

        CHECK(ss.str() == "0.5-432hello, world! 6.32");
    }

    SECTION("modifying")
    {
        [](auto... args) {
            hal::reverse::for_each([](auto& x) { x = 1; }, args...);
            hal::reverse::for_each([](auto x) { CHECK(x == 1); }, args...);
        }(1, 5, 3);
    }

    SECTION("partial application")
    {
        auto ss = std::stringstream{};
        auto write_to_ss_in_rev =
            hal::reverse::for_each([&ss](auto const& x) { ss << x; });

        write_to_ss_in_rev(2, 6.3, ' ', "hello, world!", -432);
        write_to_ss_in_rev(0.5);
        write_to_ss_in_rev();
        CHECK(ss.str() == "-432hello, world! 6.320.5");
    }

    SECTION("constexpr")
    {
        constexpr auto nop_rev = hal::reverse::for_each([](auto) {});
        static_assert((nop_rev('a', 1, 4, 2), true));
        static_assert(
            (hal::reverse::for_each([](auto) {}, nullptr, 'a', 4.3), true));
    }
}

TEST_CASE("hal::memberwise::for_each", "[HAL]")
{
    struct Foo {
        int a         = 3;
        char b        = '#';
        double c      = 5.212;
        std::string d = "Hello, World!";
    };

    struct Empty {};

    SECTION("full call & call order")
    {
        auto ss      = std::stringstream{};
        auto const f = Foo{};
        hal::memberwise::for_each([&ss](auto const& x) { ss << x; }, f);
        CHECK(ss.str() == "3#5.212Hello, World!");
    }

    SECTION("modifying")
    {
        auto f = Foo{};
        hal::memberwise::for_each([](auto& x) { x += 1; }, f);
        CHECK(f.a == 4);
        CHECK(f.b == '$');
        CHECK(f.c == 5.212 + 1.);
        CHECK(f.d == "Hello, World!\1");
    }

    SECTION("partial application")
    {
        auto ss = std::stringstream{};
        auto write_to_ss =
            hal::memberwise::for_each([&ss](auto const& x) { ss << x; });

        auto const f = Foo{};
        write_to_ss(f);
        write_to_ss(Foo{});
        write_to_ss(Empty{});
        CHECK(ss.str() == "3#5.212Hello, World!3#5.212Hello, World!");
    }

    SECTION("constexpr")
    {
        constexpr auto nop = hal::memberwise::for_each([](auto) {});
        constexpr auto e   = Empty{};
        static_assert((nop(e), true));
        static_assert((nop(Empty{}), true));
        static_assert((hal::memberwise::for_each([](auto) {}, Empty{}), true));
    }
}

TEST_CASE("hal::reverse::memberwise::for_each", "[HAL]")
{
    struct Foo {
        int a         = 3;
        char b        = '#';
        double c      = 5.212;
        std::string d = "Hello, World!";
    };

    struct Empty {};

    SECTION("full call & call order")
    {
        auto ss      = std::stringstream{};
        auto const f = Foo{};
        hal::reverse::memberwise::for_each([&ss](auto const& x) { ss << x; },
                                           f);
        CHECK(ss.str() == "Hello, World!5.212#3");
    }

    SECTION("modifying")
    {
        auto f = Foo{};
        hal::reverse::memberwise::for_each([](auto& x) { x += 1; }, f);
        CHECK(f.a == 4);
        CHECK(f.b == '$');
        CHECK(f.c == 5.212 + 1.);
        CHECK(f.d == "Hello, World!\1");
    }

    SECTION("partial application")
    {
        auto ss          = std::stringstream{};
        auto write_to_ss = hal::reverse::memberwise::for_each(
            [&ss](auto const& x) { ss << x; });

        auto const f = Foo{};
        write_to_ss(f);
        write_to_ss(Foo{});
        write_to_ss(Empty{});
        CHECK(ss.str() == "Hello, World!5.212#3Hello, World!5.212#3");
    }

    SECTION("constexpr")
    {
        constexpr auto nop = hal::reverse::memberwise::for_each([](auto) {});
        constexpr auto e   = Empty{};
        static_assert((nop(e), true));
        static_assert((nop(Empty{}), true));
        static_assert((hal::memberwise::for_each([](auto) {}, Empty{}), true));
    }
}
