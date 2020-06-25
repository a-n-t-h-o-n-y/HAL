#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("to_tuple", "[HAL]")
{
    struct Foo {
        int a    = 34;
        char b   = 'y';
        double c = 7.432;
    };
    SECTION("rvalue struct")
    {
        auto const t = hal::to_tuple(Foo{});
        CHECK(std::get<0>(t) == 34);
        CHECK(std::get<1>(t) == 'y');
        CHECK(std::get<2>(t) == 7.432);
    }
    SECTION("lvalue struct")
    {
        auto const a = Foo{};
        auto const t = hal::to_tuple(a);
        CHECK(std::get<0>(t) == 34);
        CHECK(std::get<1>(t) == 'y');
        CHECK(std::get<2>(t) == 7.432);
    }
    SECTION("constexpr")
    {
        constexpr auto t = hal::to_tuple(Foo{});
        static_assert(std::get<0>(t) == 34);
        static_assert(std::get<1>(t) == 'y');
        static_assert(std::get<2>(t) == 7.432);

        constexpr auto a  = Foo{};
        constexpr auto t2 = hal::to_tuple(a);
        static_assert(std::get<0>(t2) == 34);
        static_assert(std::get<1>(t2) == 'y');
        static_assert(std::get<2>(t2) == 7.432);
    }
    SECTION("sixteen members")
    {
        struct Bar {
            int x0  = 0;
            int x1  = 1;
            int x2  = 2;
            int x3  = 3;
            int x4  = 4;
            int x5  = 5;
            int x6  = 6;
            int x7  = 7;
            int x8  = 8;
            int x9  = 9;
            int x10 = 10;
            int x11 = 11;
            int x12 = 12;
            int x13 = 13;
            int x14 = 14;
            int x15 = 15;
        };
        auto const t = hal::to_tuple(Bar{});
        CHECK(std::get<0>(t) == 0);
        CHECK(std::get<1>(t) == 1);
        CHECK(std::get<2>(t) == 2);
        CHECK(std::get<3>(t) == 3);
        CHECK(std::get<4>(t) == 4);
        CHECK(std::get<5>(t) == 5);
        CHECK(std::get<6>(t) == 6);
        CHECK(std::get<7>(t) == 7);
        CHECK(std::get<8>(t) == 8);
        CHECK(std::get<9>(t) == 9);
        CHECK(std::get<10>(t) == 10);
        CHECK(std::get<11>(t) == 11);
        CHECK(std::get<12>(t) == 12);
        CHECK(std::get<13>(t) == 13);
        CHECK(std::get<14>(t) == 14);
        CHECK(std::get<15>(t) == 15);
    }
}
