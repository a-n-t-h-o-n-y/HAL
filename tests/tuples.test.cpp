#include <sstream>
#include <string>
#include <tuple>

#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("tuple", "[HAL]")
{
    auto const foo = std::tuple<int, double, char>{4, 3.2, 'y'};

    // return a function that will append each arg to the given string \p s.
    auto const append_to = [](std::string& s) {
        return [&s](auto x) {
            auto ss = std::stringstream{};
            ss << x;
            s.append(ss.str());
        };
    };

    SECTION("expanding tuple with apply")
    {
        auto s           = std::string{};
        auto append_each = hal::for_each(append_to(s));
        std::apply(append_each, foo);
        CHECK(s == "43.2y");
    }
}
