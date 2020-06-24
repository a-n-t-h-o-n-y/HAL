#include <cctype>
#include <functional>
#include <sstream>

#include <catch2/catch.hpp>

#include <hal.hpp>

// constexpr auto product = [](auto x, auto y) { return x * y; };
constexpr auto sum = [](auto x, auto y) { return x + y; };
TEST_CASE("adjacent_tranform", "[HAL]")
{
    SECTION("full call")
    {
        using namespace hal;
        adjacent_transform(sum, 1, 2);

        auto ss = std::stringstream{};
        auto as = [&ss](auto&&... args) {
            adjacent_transform(sum, args...);
            for_each([&ss](auto x) { ss << x << ' '; }, args...);
        };

        as(1, 2, 3, 4, 5);
        CHECK(ss.str() == "3 5 7 9 5 ");
    }
    SECTION("partial application")
    {
        using namespace hal;
        auto apply_1 = adjacent_transform(sum);
        apply_1(1, 2, '3', 4);

        auto ss = std::stringstream{};
        auto as = [&ss](auto&&... args) {
            auto apply_1 = adjacent_transform(sum);
            apply_1(args...);
            for_each([&ss](auto x) { ss << x << ' '; }, args...);
        };

        as(1, 2, 3, 4, 5);
        CHECK(ss.str() == "3 5 7 9 5 ");
    }
    SECTION("constexpr")
    {
        using namespace hal;
        constexpr auto sf = [](auto&&... args) {
            adjacent_transform(sum, std::forward<decltype(args)>(args)...);
            return hal::first(args...);
        }(1, 2, 3);
        static_assert(sf == 3);
    }
}
