#include <cmath>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <catch2/catch.hpp>

#include <halg.hpp>

TEST_CASE("count", "[HALG]")
{
    SECTION("full call")
    {
        CHECK(halg::count(4, 1, 2, 3, 4) == 1);
        CHECK(halg::count('a', 1.1, 2, 'a', 97, 'a') == 3);
    }
    SECTION("partial application")
    {
        auto count_fives = halg::count(5);
        CHECK(count_fives(5, 5, 0, 5.5, 2, 1, 'a', 5, 5.0) == 4);

        auto count_hello = halg::count("hello");
        CHECK(count_hello("hello", "not hello", "HELLO", "hello", "") == 2);
    }
}

auto is_even(int x) { return x % 2 == 0; }
auto is_even(double x) { return std::fmod(x, 2) == 0.; }

TEST_CASE("count_if", "[HALG]")
{
    SECTION("full call")
    {
        CHECK(halg::count_if([](auto const& x) { return x > 42; }, 1, 2, 3,
                             4) == 0);
        CHECK(halg::count_if([](auto const& x) { return x > 42; }, 99) == 1);
        CHECK(halg::count_if([](auto const& x) { return x > 42; }, 99, 98,
                             97) == 3);
        CHECK(halg::count_if([](auto const& x) { return is_even(x); }, 1.1, 2,
                             'a', 97, 8.0) == 2);
    }
    SECTION("partial application")
    {
        struct Foo {
            auto empty() const -> bool { return true; }
        };
        auto count_empties =
            halg::count_if([](auto const& x) { return x.empty(); });
        CHECK(count_empties(std::string{"hi"}, std::string{},
                            std::vector{1, 2, 3}, std::map<int, std::string>{},
                            Foo{}) == 3);
        CHECK(count_empties(std::vector{1}) == 0);
    }
}
