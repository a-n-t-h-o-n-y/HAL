#ifndef HALG_HPP
#define HALG_HPP
#include <type_traits>
#include <utility>

namespace halg {
namespace detail {

/* ---------------------------Function Objects -------------------------------*/

/// Function object returning given argument transparently.
struct Identity {
    template <typename T>
    constexpr auto operator()(T&& x) const noexcept -> T&&
    {
        return std::forward<T>(x);
    }
};

/* --------------------------- Reduce Helpers --------------------------------*/

// Templates can't be in nested classes.
template <typename ReduceFn, typename T, typename BinaryOp>
class Reduce_partial_application_two {
   public:
    constexpr Reduce_partial_application_two(ReduceFn reduce_fn,
                                             T init,
                                             BinaryOp binary_op)
        : reduce_fn_{std::move(reduce_fn)},
          init_{std::move(init)},
          binary_op_{std::move(binary_op)}
    {}

    // Full Application
    template <typename... Elements>
    constexpr auto operator()(Elements&&... elements) const -> T
    {
        return reduce_fn_(init_, binary_op_,
                          std::forward<Elements>(elements)...);
    }

   private:
    ReduceFn reduce_fn_;
    T init_;
    BinaryOp binary_op_;
};

// Templates can't be in nested classes.
template <typename ReduceFn, typename T>
class Reduce_partial_application_one {
   public:
    constexpr Reduce_partial_application_one(ReduceFn reduce_fn, T init)
        : reduce_fn_{std::move(reduce_fn)}, init_{std::move(init)}
    {}

    // Partial Application
    template <typename BinaryOp>
    constexpr auto operator()(BinaryOp&& binary_op) const
    {
        return [reduce_fn_ = this->reduce_fn_, init = this->init_,
                binary_op =
                    std::forward<BinaryOp>(binary_op)](auto&&... elements) {
            return reduce_fn_(init, binary_op,
                              std::forward<decltype(elements)>(elements)...);
        };
    }

    // Full Application
    template <typename BinaryOp, typename... Elements>
    constexpr auto operator()(BinaryOp&& binary_op,
                              Elements&&... elements) const -> T
    {
        return reduce_fn_(init_, std::forward<BinaryOp>(binary_op),
                          std::forward<Elements>(elements)...);
    }

   private:
    ReduceFn reduce_fn_;
    T init_;
};

/* ---------------------------------------------------------------------------*/

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

/* --------------------------------- reduce --------------------------------- */
template <typename T, typename BinaryOp, typename... Elements>
constexpr auto reduce(T init, BinaryOp&& binary_op, Elements&&... elements) -> T
{
    [[maybe_unused]] auto x = ((init = binary_op(init, elements), true) && ...);
    return init;
}

template <typename T>
constexpr auto reduce(T&& init)
{
    return detail::Reduce_partial_application_one{
        [](auto&& i, auto&& binary_op, auto&&... elements) {
            return reduce(std::forward<decltype(i)>(i),
                          std::forward<decltype(binary_op)>(binary_op),
                          std::forward<decltype(elements)>(elements)...);
        },
        std::forward<T>(init)};
}

template <typename T, typename BinaryOp>
constexpr auto reduce(T&& init, BinaryOp&& binary_op)
{
    return detail::Reduce_partial_application_two{
        [](auto&& i, auto&& binary_op, auto&&... elements) {
            return reduce(std::forward<decltype(i)>(i),
                          std::forward<decltype(binary_op)>(binary_op),
                          std::forward<decltype(elements)>(elements)...);
        },
        std::forward<T>(init), std::forward<BinaryOp>(binary_op)};
}

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
    auto foo                      = 0;
    [[maybe_unused]] auto const i = (foo = ... = (func(elements), 0));
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
    [[maybe_unused]] auto const i =
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
    [[maybe_unused]] auto const i =
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

/* --------------------------------- reduce --------------------------------- */
template <typename T, typename BinaryOp, typename... Elements>
constexpr auto reduce(T init, BinaryOp&& binary_op, Elements&&... elements) -> T
{
    auto foo = 0;
    [[maybe_unused]] auto const i =
        (foo = ... = (init = binary_op(init, elements), true));
    return init;
}

template <typename T>
constexpr auto reduce(T&& init)
{
    return detail::Reduce_partial_application_one{
        [](auto&& i, auto&& binary_op, auto&&... elements) {
            return reverse::reduce(
                std::forward<decltype(i)>(i),
                std::forward<decltype(binary_op)>(binary_op),
                std::forward<decltype(elements)>(elements)...);
        },
        std::forward<T>(init)};
}

template <typename T, typename BinaryOp>
constexpr auto reduce(T&& init, BinaryOp&& binary_op)
{
    return detail::Reduce_partial_application_two{
        [](auto&& i, auto&& binary_op, auto&&... elements) {
            return reverse::reduce(
                std::forward<decltype(i)>(i),
                std::forward<decltype(binary_op)>(binary_op),
                std::forward<decltype(elements)>(elements)...);
        },
        std::forward<T>(init), std::forward<BinaryOp>(binary_op)};
}

}  // namespace reverse
}  // namespace halg
#endif  // HALG_HPP
