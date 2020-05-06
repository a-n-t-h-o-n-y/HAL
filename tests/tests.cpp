#include <catch2/catch.hpp>

#include <halg.hpp>

template <typename... Args>
auto foo(Args&&... args)
{
    return halg::for_each([](auto i) { REQUIRE(i == i); },
                          std::forward<Args>(args)...);
}

TEST_CASE("for_each", "[HALG]")
{
    auto glob        = 0;
    auto add_to_glob = halg::for_each([&glob](auto const& i) { glob += i; });

    add_to_glob(1, 45, 2, 'a');
    add_to_glob(5, -7);
    REQUIRE(glob == (0 + 1 + 45 + 2 + 'a' + 5 - 7));

    halg::for_each([&glob](auto const& i) { glob -= i; }, 1, 45, 2, 'a', 5, -7);
    REQUIRE(glob == 0);

    foo(3.2, "help", 23, -6, 4.f);
}
