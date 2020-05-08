#include <sstream>

#include <catch2/catch.hpp>

#include <halg.hpp>

TEST_CASE("tranform", "[HALG]")
{
    SECTION("full call")
    {
        // l-values
        auto a = 'a';
        auto b = 2;
        auto c = 3.23;
        auto d = 4;
        halg::transform([i = 0](auto x) mutable { return x + ++i; }, a, b, c,
                        d);
        auto ss = std::stringstream{};
        halg::for_each([&ss](auto x) { ss << x; }, a, b, c, d);
        CHECK(ss.str() == "b46.238");

        auto e = 'i';
        auto f = 'f';
        auto g = 'm';
        auto h = 'm';
        auto i = 'p';
        halg::transform([](auto x) { return x - 1; }, e, f, g, h, i);
        ss.str(std::string{});
        halg::for_each([&ss](auto x) { ss << x; }, e, f, g, h, i);
        CHECK(ss.str() == "hello");

        // r-values
        halg::transform([](auto x) { return x + 1; }, 1, '2', 3, 4);
    }
    SECTION("partial application")
    {
        auto a              = 'a';
        auto b              = 'b';
        auto c              = 3.23;
        auto d              = -4;
        auto increment_each = halg::transform([](auto x) { return x + 1; });
        increment_each(a, b, c, d);
        CHECK(a == 'b');
        CHECK(b == 'c');
        CHECK(c == 4.23);
        CHECK(d == -3);
    }
}

TEST_CASE("reverse::tranform", "[HALG]")
{
    SECTION("full call")
    {
        // l-values
        auto a = 'a';
        auto b = 2;
        auto c = 3.23;
        auto d = 4;
        halg::reverse::transform([i = 0](auto x) mutable { return x + ++i; }, a,
                                 b, c, d);
        auto ss = std::stringstream{};
        halg::for_each([&ss](auto x) { ss << x; }, a, b, c, d);
        CHECK(ss.str() == "e55.235");

        auto e = 'i';
        auto f = 'f';
        auto g = 'm';
        auto h = 'm';
        auto i = 'p';
        halg::reverse::transform([](auto x) { return x - 1; }, e, f, g, h, i);
        ss.str(std::string{});
        halg::for_each([&ss](auto x) { ss << x; }, e, f, g, h, i);
        CHECK(ss.str() == "hello");

        // r-values
        halg::reverse::transform([](auto x) { return x + 1; }, 1, '2', 3, 4);
    }
    SECTION("partial application")
    {
        auto a = 'a';
        auto b = 'b';
        auto c = 3.23;
        auto d = -4;
        auto increment_each =
            halg::reverse::transform([](auto x) { return x + 1; });
        increment_each(a, b, c, d);
        CHECK(a == 'b');
        CHECK(b == 'c');
        CHECK(c == 4.23);
        CHECK(d == -3);
    }
}
