#include <sstream>

#include <catch2/catch.hpp>

#include <halg.hpp>

TEST_CASE("for_each", "[HALG]")
{
    SECTION("full call")
    {
        auto ss = std::stringstream{};

        halg::for_each([&ss](auto const& x) { ss << x; }, 2, 6.3, ' ',
                       "hello, world!", -432, 0.5);

        CHECK(ss.str() == "26.3 hello, world!-4320.5");
    }

    SECTION("partial application")
    {
        auto ss          = std::stringstream{};
        auto write_to_ss = halg::for_each([&ss](auto const& x) { ss << x; });

        write_to_ss(2, 6.3, ' ', "hello, world!", -432);
        write_to_ss(0.5);
        write_to_ss();
        CHECK(ss.str() == "26.3 hello, world!-4320.5");
    }

    SECTION("constexpr")
    {
        constexpr auto nop = halg::for_each([](auto) {});
        static_assert((nop('a', 1, 4, 2), true));
        static_assert((halg::for_each([](auto) {}, nullptr, 'a', 4.3), true));
    }
}

TEST_CASE("reverse::for_each", "[HALG]")
{
    SECTION("full call")
    {
        auto ss = std::stringstream{};

        halg::reverse::for_each([&ss](auto const& x) { ss << x; }, 2, 6.3, ' ',
                                "hello, world!", -432, 0.5);

        CHECK(ss.str() == "0.5-432hello, world! 6.32");
    }

    SECTION("partial application")
    {
        auto ss = std::stringstream{};
        auto write_to_ss_in_rev =
            halg::reverse::for_each([&ss](auto const& x) { ss << x; });

        write_to_ss_in_rev(2, 6.3, ' ', "hello, world!", -432);
        write_to_ss_in_rev(0.5);
        write_to_ss_in_rev();
        CHECK(ss.str() == "-432hello, world! 6.320.5");
    }

    SECTION("constexpr")
    {
        constexpr auto nop_rev = halg::reverse::for_each([](auto) {});
        static_assert((nop_rev('a', 1, 4, 2), true));
        static_assert(
            (halg::reverse::for_each([](auto) {}, nullptr, 'a', 4.3), true));
    }
}
