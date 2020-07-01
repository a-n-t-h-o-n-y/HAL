#include <sstream>
#include <string>
#include <tuple>

#include <catch2/catch.hpp>

#include <hal.hpp>

TEST_CASE("hal::partial_reduce", "[HAL]")
{
    SECTION("empty parameter pack")
    {
        [](auto... xs) { hal::partial_reduce(0, std::plus<>{}, xs...); }();
    }

    SECTION("full call")
    {
        [](auto... xs) {
            hal::partial_reduce(0, std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial application")
    {
        auto init_pr = hal::partial_reduce(0);
        [&](auto... xs) {
            init_pr(std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);

        auto ps = hal::partial_reduce(0, std::plus<>{});
        [&](auto... xs) {
            ps(xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial_sum")
    {
        [&](auto... xs) {
            hal::partial_sum(xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 3);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == 2);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 7);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial_difference")
    {
        [&](auto... xs) {
            hal::partial_difference(xs...);
            CHECK(hal::get<0>(xs...) == -1);
            CHECK(hal::get<1>(xs...) == -3);
            CHECK(hal::get<2>(xs...) == -6);
            CHECK(hal::get<3>(xs...) == -2);
            CHECK(hal::get<4>(xs...) == -5.2);
            CHECK(hal::get<5>(xs...) == -7);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial_product")
    {
        [&](auto... xs) {
            hal::partial_product(xs...);
            CHECK(hal::get<0>(xs...) == 1);
            CHECK(hal::get<1>(xs...) == 2);
            CHECK(hal::get<2>(xs...) == 6);
            CHECK(hal::get<3>(xs...) == -24);
            CHECK(hal::get<4>(xs...) == -24 * 3.2);
            CHECK(hal::get<5>(xs...) == -152);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial_quotient")
    {
        [&](auto... xs) {
            hal::partial_quotient(xs...);
            CHECK(hal::get<0>(xs...) == 1.);
            CHECK(hal::get<1>(xs...) == .5);
            CHECK(hal::get<2>(xs...) == .5 / 3.);
            CHECK(hal::get<3>(xs...) == (.5 / 3.) / -4.);
            CHECK(hal::get<4>(xs...) == ((.5 / 3.) / -4.) / 0.000002);
            CHECK(hal::get<5>(xs...) == -694);
        }(1., 2., 3., -4., 0.000002, 30);
    }
}

TEST_CASE("hal::reverse::partial_reduce", "[HAL]")
{
    SECTION("full call")
    {
        [](auto... xs) {
            hal::reverse::partial_reduce(0, std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 7);
            CHECK(hal::get<1>(xs...) == 6);
            CHECK(hal::get<2>(xs...) == 4);
            CHECK(hal::get<3>(xs...) == 1);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 2);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial application")
    {
        auto init_pr = hal::reverse::partial_reduce(0);
        [&](auto... xs) {
            init_pr(std::plus<>{}, xs...);
            CHECK(hal::get<0>(xs...) == 7);
            CHECK(hal::get<1>(xs...) == 6);
            CHECK(hal::get<2>(xs...) == 4);
            CHECK(hal::get<3>(xs...) == 1);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 2);
        }(1, 2, 3, -4, 3.2, 2);

        auto ps = hal::reverse::partial_reduce(0, std::plus<>{});
        [&](auto... xs) {
            ps(xs...);
            CHECK(hal::get<0>(xs...) == 7);
            CHECK(hal::get<1>(xs...) == 6);
            CHECK(hal::get<2>(xs...) == 4);
            CHECK(hal::get<3>(xs...) == 1);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 2);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial_sum")
    {
        [&](auto... xs) {
            hal::reverse::partial_sum(xs...);
            CHECK(hal::get<0>(xs...) == 7);
            CHECK(hal::get<1>(xs...) == 6);
            CHECK(hal::get<2>(xs...) == 4);
            CHECK(hal::get<3>(xs...) == 1);
            CHECK(hal::get<4>(xs...) == 5.2);
            CHECK(hal::get<5>(xs...) == 2);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial_difference")
    {
        [&](auto... xs) {
            hal::reverse::partial_difference(xs...);
            CHECK(hal::get<0>(xs...) == -7);
            CHECK(hal::get<1>(xs...) == -6);
            CHECK(hal::get<2>(xs...) == -4);
            CHECK(hal::get<3>(xs...) == -1);
            CHECK(hal::get<4>(xs...) == -5.2);
            CHECK(hal::get<5>(xs...) == -2);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial_product")
    {
        [&](auto... xs) {
            hal::reverse::partial_product(xs...);
            CHECK(hal::get<0>(xs...) == -144);
            CHECK(hal::get<1>(xs...) == -144);
            CHECK(hal::get<2>(xs...) == -72);
            CHECK(hal::get<3>(xs...) == -24);
            CHECK(hal::get<4>(xs...) == 2 * 3.2);
            CHECK(hal::get<5>(xs...) == 2);
        }(1, 2, 3, -4, 3.2, 2);
    }

    SECTION("partial_quotient")
    {
        [&](auto... xs) {
            hal::reverse::partial_quotient(xs...);
            CHECK(hal::get<0>(xs...) == ((((1 / 30.) / .000002) / -4) / 3) / 2);
            CHECK(hal::get<1>(xs...) == ((((1 / 30.) / .000002) / -4) / 3) / 2);
            CHECK(hal::get<2>(xs...) == (((1 / 30.) / .000002) / -4) / 3);
            CHECK(hal::get<3>(xs...) == ((1 / 30.) / .000002) / -4);
            CHECK(hal::get<4>(xs...) == (1 / 30.) / .000002);
            CHECK(hal::get<5>(xs...) == 1 / 30.);
        }(1., 2., 3., -4., 0.000002, 30.);
    }
}

namespace {
struct Foo {
    int a    = 3;
    char b   = '#';
    double c = 5.212;
};

struct Empty {};
}  // namespace

TEST_CASE("hal::memberwise::partial_reduce", "[HAL]")
{
    SECTION("empty")
    {
        auto e = Empty{};
        hal::memberwise::partial_reduce(0, std::plus<>{}, e);
        hal::memberwise::partial_reduce(0, std::plus<>{}, Empty{});
    }

    SECTION("call order")
    {
        auto f        = Foo{};
        auto ss       = std::stringstream{};
        auto appender = [&ss](auto const&, auto const& x) {
            ss << x;
            return x;
        };
        hal::memberwise::partial_reduce(0, appender, f);
        CHECK(ss.str() == "3#5.212");
    }

    SECTION("full call")
    {
        auto f = Foo{};
        hal::memberwise::partial_reduce(0, std::plus<>{}, f);
        CHECK(f.a == Foo{}.a);
        CHECK(f.b == (Foo{}.a + Foo{}.b));
        CHECK(f.c == (Foo{}.a + Foo{}.b + Foo{}.c));

        hal::memberwise::partial_reduce(0, std::plus<>{}, Foo{});
        hal::memberwise::partial_reduce(0, std::plus<>{}, Empty{});
    }

    SECTION("partial application")
    {
        auto f       = Foo{};
        auto init_pr = hal::memberwise::partial_reduce(0);
        init_pr(std::plus<>{}, f);
        CHECK(f.a == Foo{}.a);
        CHECK(f.b == (Foo{}.a + Foo{}.b));
        CHECK(f.c == (Foo{}.a + Foo{}.b + Foo{}.c));

        init_pr(std::plus<>{}, Foo{});
        init_pr(std::plus<>{}, Empty{});
    }

    SECTION("partial_sum")
    {
        auto f = Foo{};
        hal::memberwise::partial_sum(f);
        CHECK(f.a == Foo{}.a);
        CHECK(f.b == (Foo{}.a + Foo{}.b));
        CHECK(f.c == (Foo{}.a + Foo{}.b + Foo{}.c));

        hal::memberwise::partial_sum(Empty{});
    }

    SECTION("partial_difference")
    {
        auto f = Foo{};
        hal::memberwise::partial_difference(f);
        CHECK(f.a == 0 - Foo{}.a);
        CHECK(f.b == (0 - Foo{}.a - Foo{}.b));
        CHECK(f.c == (0 - Foo{}.a - Foo{}.b - Foo{}.c));

        hal::memberwise::partial_difference(Empty{});
    }

    SECTION("partial_product")
    {
        auto f = Foo{};
        hal::memberwise::partial_product(f);
        CHECK(f.a == Foo{}.a);
        CHECK(f.b == (Foo{}.a * Foo{}.b));
        CHECK(f.c == (Foo{}.a * Foo{}.b * Foo{}.c));

        hal::memberwise::partial_product(Empty{});
    }

    SECTION("partial_quotient")
    {
        auto f = Foo{};
        hal::memberwise::partial_quotient(f);
        CHECK(f.a == 1 / Foo{}.a);
        CHECK(f.b == (1 / Foo{}.a / Foo{}.b));
        CHECK(f.c == (1 / Foo{}.a / Foo{}.b / Foo{}.c));

        hal::memberwise::partial_quotient(Empty{});
    }
}

TEST_CASE("hal::reverse::memberwise::partial_reduce", "[HAL]")
{
    SECTION("empty parameter pack")
    {
        auto e = Empty{};
        hal::reverse::memberwise::partial_reduce(0, std::plus<>{}, e);
        hal::reverse::memberwise::partial_reduce(0, std::plus<>{}, Empty{});
    }

    SECTION("call order")
    {
        auto f        = Foo{};
        auto ss       = std::stringstream{};
        auto appender = [&ss](auto const&, auto const& x) {
            ss << x;
            return x;
        };
        hal::reverse::memberwise::partial_reduce(0, appender, f);
        CHECK(ss.str() == "5.212#3");
    }

    SECTION("full call")
    {
        auto f = Foo{};
        hal::reverse::memberwise::partial_reduce(0, std::plus<>{}, f);
        CHECK(f.a == (int)(Foo{}.c + Foo{}.b + Foo{}.a));
        CHECK(f.b == (int)(Foo{}.c + Foo{}.b));
        CHECK(f.c == Foo{}.c);

        hal::reverse::memberwise::partial_reduce(0, std::plus<>{}, Foo{});
        hal::reverse::memberwise::partial_reduce(0, std::plus<>{}, Empty{});
    }

    SECTION("partial application")
    {
        auto f       = Foo{};
        auto init_pr = hal::reverse::memberwise::partial_reduce(0);
        init_pr(std::plus<>{}, f);
        CHECK(f.a == (int)(Foo{}.c + Foo{}.b + Foo{}.a));
        CHECK(f.b == (int)(Foo{}.c + Foo{}.b));
        CHECK(f.c == (Foo{}.c));

        init_pr(std::plus<>{}, Foo{});
        init_pr(std::plus<>{}, Empty{});
    }

    SECTION("partial_sum")
    {
        auto f = Foo{};
        hal::reverse::memberwise::partial_sum(f);
        CHECK(f.a == (int)(Foo{}.c + Foo{}.b + Foo{}.a));
        CHECK(f.b == (int)(Foo{}.c + Foo{}.b));
        CHECK(f.c == Foo{}.c);

        hal::memberwise::partial_sum(Empty{});
    }

    SECTION("partial_difference")
    {
        auto f = Foo{};
        hal::reverse::memberwise::partial_difference(f);
        CHECK(f.a == (int)(0 - Foo{}.c - Foo{}.b - Foo{}.a));
        CHECK(f.b == (int)(0 - Foo{}.c - Foo{}.b));
        CHECK(f.c == 0 - Foo{}.c);

        hal::reverse::memberwise::partial_difference(Empty{});
    }

    SECTION("partial_product")
    {
        struct Bar {
            int a    = 2;
            char b   = '\5';
            double c = 0.4;
        } b;

        hal::reverse::memberwise::partial_product(b);
        CHECK(b.a == (int)(Bar{}.c * Bar{}.b * Bar{}.a));
        CHECK(b.b == (int)(Bar{}.c * Bar{}.b));
        CHECK(b.c == Bar{}.c);

        hal::reverse::memberwise::partial_product(Empty{});
    }

    SECTION("partial_quotient")
    {
        auto f = Foo{};
        hal::memberwise::partial_quotient(f);
        CHECK(f.a == (int)(1 / Foo{}.c / Foo{}.b / Foo{}.a));
        CHECK(f.b == (int)(1 / Foo{}.c / Foo{}.b));
        CHECK(f.c == (int)(1 / Foo{}.c));

        hal::memberwise::partial_quotient(Empty{});
    }
}
