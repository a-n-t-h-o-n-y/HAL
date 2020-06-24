#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("adjacent_find", "[HAL]")
{
    SECTION("contains adjacent equal")
    {
        auto index = hal::adjacent_find(1, 2, 3, 3, 4, 5);
        // Returns the first index of the equal pair
        CHECK(index == 2);
    }
    SECTION("no adjacent equal")
    {
        auto index = hal::adjacent_find(1, 2, 3, 4, 5);
        // Returns index 4 instead of 5, because 5 is not a valid index checked.
        CHECK(index == 4);
    }
    SECTION("contains multiple adjacent equal")
    {
        auto index = hal::adjacent_find(1, 2, 3, 3, 3, 3, 4, 5);
        CHECK(index == 2);
    }
    SECTION("contains multiple adjacent equal pairs")
    {
        auto index = hal::adjacent_find(1, 2, 3, 3, 4, 5, 5, 2, 3, 3);
        CHECK(index == 2);
    }
    SECTION("constexpr")
    {
        static_assert(hal::adjacent_find(1, 2, 3, 3, 4, 5, 5, 2, 3, 3) == 2);
    }
}
