#include <sstream>
#include <string>
#include <tuple>

#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("tuple", "[HAL]")
{
    SECTION("expanding tuple with apply")
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

        auto s           = std::string{};
        auto append_each = hal::for_each(append_to(s));
        std::apply(append_each, foo);
        CHECK(s == "43.2y");
    }

    SECTION("to_tuple", "[HAL]")
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
    SECTION("to_ref_tuple")
    {
        struct Foo {
            int a    = 34;
            char b   = '#';  // 35
            double c = 7.432;
        };
        SECTION("general")
        {
            auto a = Foo{};
            auto t = hal::to_ref_tuple(a);
            std::apply(
                [](auto&&... x) {
                    hal::partial_sum(std::forward<decltype(x)>(x)...);
                },
                t);
            CHECK(std::get<0>(t) == 34);
            CHECK(std::get<1>(t) == 'E');
            CHECK(std::get<2>(t) == 76.432);
            CHECK(a.a == 34);
            CHECK(a.b == 'E');
            CHECK(a.c == 76.432);
        }
    }
    SECTION("algorithm on struct members")
    {
        struct Baz {
            int value_a{8};
            int value_b{16};
            int value_c{32};
        } constexpr baz;
        SECTION("r-value tuple")
        {
            auto const sum = hal::reduce(0, std::plus<>{});
            auto result    = std::apply(sum, hal::to_tuple(baz));
            CHECK(result == 56);
        }
        SECTION("l-value tuple")  // Can't use modifying methods on r-values
        {
            auto ps = [](auto&&... elements) {
                hal::partial_sum(std::forward<decltype(elements)>(elements)...);
            };
            auto tup = hal::to_tuple(baz);
            std::apply(ps, tup);
            auto res = hal::from_tuple<Baz>(tup);
            CHECK(res.value_a == 8);
            CHECK(res.value_b == 24);
            CHECK(res.value_c == 56);
        }
        SECTION("constexpr")
        {
            constexpr auto sum    = hal::reduce(0, std::plus<>{});
            constexpr auto result = std::apply(sum, hal::to_tuple(baz));
            static_assert(result == 56);
        }
    }
    SECTION("from_tuple")
    {
        struct Baz {
            int value_a;
            int value_b;
            int value_c;
        };
        auto b = hal::from_tuple<Baz>(std::make_tuple(1, 2, 3));
        CHECK(b.value_a == 1);
        CHECK(b.value_b == 2);
        CHECK(b.value_c == 3);
        SECTION("constexpr")
        {
            constexpr auto b = hal::from_tuple<Baz>(std::make_tuple(1, 2, 3));
            static_assert(b.value_a == 1);
            static_assert(b.value_b == 2);
            static_assert(b.value_c == 3);
        }
    }
}
