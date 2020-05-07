#ifndef HALG_HPP
#define HALG_HPP
#include <utility>

namespace halg {

/* -------------------------------- for_each -------------------------------- */
template <typename UnaryFunction, typename... Elements>
void for_each(UnaryFunction&& func, Elements&&... elements)
{
    (func(std::forward<Elements>(elements)), ...);
}

template <typename UnaryFunction>
auto for_each(UnaryFunction&& func)
{
    return [f = std::forward<UnaryFunction>(func)](auto&&... elements) {
        return for_each(f, std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- all_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
auto all_of(UnaryPredicate&& predicate, Elements&&... elements) -> bool
{
    return (predicate(std::forward<Elements>(elements)) && ...);
}

template <typename UnaryPredicate>
auto all_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return all_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- any_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
auto any_of(UnaryPredicate&& predicate, Elements&&... elements) -> bool
{
    return (predicate(std::forward<Elements>(elements)) || ...);
}

template <typename UnaryPredicate>
auto any_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return any_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

/* -------------------------------- none_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
auto none_of(UnaryPredicate&& predicate, Elements&&... elements) -> bool
{
    return !any_of(std::forward<UnaryPredicate>(predicate),
                   std::forward<Elements>(elements)...);
}

template <typename UnaryPredicate>
auto none_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return none_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

namespace reverse {

/* --------------------------- Reversed for_each ---------------------------- */
template <typename UnaryFunction, typename... Elements>
void for_each(UnaryFunction&& func, Elements&&... elements)
{
    auto foo = 0;
    (foo = ... = (func(elements), 0));
}

template <typename UnaryFunction>
auto for_each(UnaryFunction&& func)
{
    return [func = std::forward<UnaryFunction>(func)](auto&&... elements) {
        return reverse::for_each(func,
                                 std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- all_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
auto all_of(UnaryPredicate&& predicate, Elements&&... elements) -> bool
{
    auto x = true;
    int foo;
    (foo = ... = (x = x && predicate(std::forward<Elements>(elements))));
    return x;
}

template <typename UnaryPredicate>
auto all_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return reverse::all_of(f,
                               std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- any_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
auto any_of(UnaryPredicate&& predicate, Elements&&... elements) -> bool
{
    auto x = false;
    int foo;
    (foo = ... = (x = x || predicate(std::forward<Elements>(elements))));
    return x;
}

template <typename UnaryPredicate>
auto any_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return reverse::any_of(f,
                               std::forward<decltype(elements)>(elements)...);
    };
}

/* -------------------------------- none_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
auto none_of(UnaryPredicate&& predicate, Elements&&... elements) -> bool
{
    return !reverse::any_of(std::forward<UnaryPredicate>(predicate),
                            std::forward<Elements>(elements)...);
}

template <typename UnaryPredicate>
auto none_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return reverse::none_of(f,
                                std::forward<decltype(elements)>(elements)...);
    };
}

}  // namespace reverse
}  // namespace halg
#endif  // HALG_HPP
