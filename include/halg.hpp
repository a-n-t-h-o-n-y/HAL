#ifndef HALG_HPP
#define HALG_HPP
#include <utility>

namespace halg {

/* -------------------------------- for_each -------------------------------- */
template <typename UnaryFunction, typename... Elements>
void for_each(UnaryFunction&& func, Elements&&... elements)
{
    (func(elements), ...);
}

template <typename UnaryFunction>
auto for_each(UnaryFunction&& func)
{
    return [func = std::forward<UnaryFunction>(func)](auto&&... elements) {
        return for_each(func, std::forward<decltype(elements)>(elements)...);
    };
}

/* -------------------------------- for_each -------------------------------- */

namespace reverse {

/* --------------------------- Reversed for_each ---------------------------- */
template <typename UnaryFunction, typename... Elements>
void for_each(UnaryFunction&& func, Elements&&... elements)
{
    int foo;
    (foo = ... = (func(elements), 0));
}

template <typename UnaryFunction>
auto for_each(UnaryFunction&& func)
{
    return [func = std::forward<UnaryFunction>(func)](auto&&... elements) {
        return for_each(func, std::forward<decltype(elements)>(elements)...);
    };
}

}  // namespace reverse
}  // namespace halg
#endif  // HALG_HPP
