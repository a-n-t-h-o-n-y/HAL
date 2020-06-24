#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("adjacent_difference", "[HAL]")
{
    [](auto... args) {
        hal::adjacent_difference(args...);
        CHECK(hal::get<0>(args...) == 4);
        CHECK(hal::get<1>(args...) == -2);
        CHECK(hal::get<2>(args...) == 3);
    }(1, 5, 3);
}
