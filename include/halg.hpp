#ifndef HALG_HPP
#define HALG_HPP
#include <type_traits>
#include <utility>

namespace halg {
namespace detail {

/// Function object returning given argument transparently.
struct Identity {
    template <typename T>
    constexpr auto operator()(T&& x) const noexcept -> T&&
    {
        return std::forward<T>(x);
    }
};

}  // namespace detail

/* -------------------------------- for_each -------------------------------- */
template <typename UnaryFunction, typename... Elements>
constexpr void for_each(UnaryFunction&& func, Elements&&... elements)
{
    (func(std::forward<Elements>(elements)), ...);
}

template <typename UnaryFunction>
constexpr auto for_each(UnaryFunction&& func)
{
    return [f = std::forward<UnaryFunction>(func)](auto&&... elements) {
        return for_each(f, std::forward<decltype(elements)>(elements)...);
    };
}

// Is_transformation_fn
// TODO reduce(<Init_t(Arg const&, Init_t init, Args...)>) -> Init_t

/* --------------------------------- all_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
constexpr auto all_of(UnaryPredicate&& predicate, Elements&&... elements)
    -> bool
{
    return (predicate(std::forward<Elements>(elements)) && ...);
}

template <typename UnaryPredicate>
constexpr auto all_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return all_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto all(Elements&&... elements) -> bool
{
    return all_of(detail::Identity{}, std::forward<Elements>(elements)...);
}

/* --------------------------------- any_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
constexpr auto any_of(UnaryPredicate&& predicate, Elements&&... elements)
    -> bool
{
    return (predicate(std::forward<Elements>(elements)) || ...);
}

template <typename UnaryPredicate>
constexpr auto any_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return any_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto any(Elements&&... elements) -> bool
{
    return any_of(detail::Identity{}, std::forward<Elements>(elements)...);
}

/* -------------------------------- none_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
constexpr auto none_of(UnaryPredicate&& predicate, Elements&&... elements)
    -> bool
{
    return !any_of(std::forward<UnaryPredicate>(predicate),
                   std::forward<Elements>(elements)...);
}

template <typename UnaryPredicate>
constexpr auto none_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return none_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto none(Elements&&... elements) -> bool
{
    return none_of(detail::Identity{}, std::forward<Elements>(elements)...);
}

namespace reverse {

/* --------------------------- Reversed for_each ---------------------------- */
template <typename UnaryFunction, typename... Elements>
constexpr void for_each(UnaryFunction&& func, Elements&&... elements)
{
    auto foo = 0;
    (foo = ... = (func(elements), 0));
}

template <typename UnaryFunction>
constexpr auto for_each(UnaryFunction&& func)
{
    return [func = std::forward<UnaryFunction>(func)](auto&&... elements) {
        return reverse::for_each(func,
                                 std::forward<decltype(elements)>(elements)...);
    };
}

/* ---------------------------- Reversed all_of ----------------------------- */
template <typename UnaryPredicate, typename... Elements>
constexpr auto all_of(UnaryPredicate&& predicate, Elements&&... elements)
    -> bool
{
    auto x   = true;
    auto foo = 0;
    (foo = ... = (x = x && predicate(std::forward<Elements>(elements))));
    return x;
}

template <typename UnaryPredicate>
constexpr auto all_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return reverse::all_of(f,
                               std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto all(Elements&&... elements) -> bool
{
    return reverse::all_of(detail::Identity{},
                           std::forward<Elements>(elements)...);
}

/* ---------------------------- Reversed any_of ----------------------------- */
template <typename UnaryPredicate, typename... Elements>
constexpr auto any_of(UnaryPredicate&& predicate, Elements&&... elements)
    -> bool
{
    auto x   = false;
    auto foo = 0;
    (foo = ... = (x = x || predicate(std::forward<Elements>(elements))));
    return x;
}

template <typename UnaryPredicate>
constexpr auto any_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return reverse::any_of(f,
                               std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto any(Elements&&... elements) -> bool
{
    return reverse::any_of(detail::Identity{},
                           std::forward<Elements>(elements)...);
}

/* --------------------------- Reversed none_of ----------------------------- */
template <typename UnaryPredicate, typename... Elements>
constexpr auto none_of(UnaryPredicate&& predicate, Elements&&... elements)
    -> bool
{
    return !reverse::any_of(std::forward<UnaryPredicate>(predicate),
                            std::forward<Elements>(elements)...);
}

template <typename UnaryPredicate>
constexpr auto none_of(UnaryPredicate&& predicate)
{
    return [f = std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return reverse::none_of(f,
                                std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto none(Elements&&... elements) -> bool
{
    return reverse::none_of(detail::Identity{},
                            std::forward<Elements>(elements)...);
}

}  // namespace reverse
}  // namespace halg
#endif  // HALG_HPP
