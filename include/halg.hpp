#ifndef HALG_HPP
#define HALG_HPP
#include <cstddef>
#include <functional>
#include <tuple>
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

/// Provides a single variadic call operator to finish the call to \p fn.
template <typename Fn>
class Partial_application_one {
   public:
    constexpr Partial_application_one(Fn fn) : fn_{std::move(fn)} {}

   public:
    template <typename... Elements>
    constexpr auto operator()(Elements&&... elements) const
    {
        return fn_(std::forward<Elements>(elements)...);
    }

   protected:
    Fn fn_;
};

/// Provides call-op that binds one argument and returns a variadic callable.
template <typename Fn>
class Partial_application_two : public Partial_application_one<Fn> {
   private:
    using Base_t = Partial_application_one<Fn>;

   public:
    constexpr Partial_application_two(Fn fn) : Base_t{std::move(fn)} {}

   public:
    using Base_t::operator();

    template <typename T>
    constexpr auto operator()(T x) const
    {
        return [fn_ = Base_t::fn_, x = std::move(x)](auto&&... elements) {
            return fn_(x, std::forward<decltype(elements)>(elements)...);
        };
    }
};

/// Provides call-op that binds two arguments and returns a variadic callable.
template <typename Fn>
class Partial_application_three : public Partial_application_two<Fn> {
   private:
    using Base_t = Partial_application_two<Fn>;

   public:
    using Base_t::operator();

    constexpr Partial_application_three(Fn fn) : Base_t{std::move(fn)} {}

   public:
    template <typename T, typename U>
    constexpr auto operator()(T x, U y) const
    {
        return [fn_ = Base_t::fn_, x = std::move(x),
                y = std::move(y)](auto&&... elements) {
            return fn_(x, y, std::forward<decltype(elements)>(elements)...);
        };
    }

    template <typename T>
    constexpr auto operator()(T x) const
    {
        return [fn_ = Base_t::fn_, x = std::move(x)](auto&& y) {
            return [fn_ = fn_, x = x, y = y](auto&&... elements) {
                return fn_(x, y, std::forward<decltype(elements)>(elements)...);
            };
        };
    }
};

}  // namespace detail

/* -------------------------------- for_each -------------------------------- */
template <typename UnaryFunction, typename... Elements>
constexpr auto for_each(UnaryFunction&& func, Elements&&... elements) -> void
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
    ((init = binary_op(init, std::forward<Elements>(elements))), ...);
    return init;
}

template <typename T>
constexpr auto reduce(T init)
{
    return detail::Partial_application_two{
        [init = std::move(init)](auto&& binary_op, auto&&... elements) {
            return reduce(init, std::forward<decltype(binary_op)>(binary_op),
                          std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename BinaryOp>
constexpr auto reduce(T init, BinaryOp binary_op)
{
    return detail::Partial_application_one{
        [init      = std::move(init),
         binary_op = std::move(binary_op)](auto&&... elements) {
            return reduce(init, binary_op,
                          std::forward<decltype(elements)>(elements)...);
        }};
}

/* ------------------------------- transform -------------------------------- */
template <typename MapFn, typename... Elements>
constexpr auto transform(MapFn&& map_fn, Elements&&... elements) -> void
{
    ((elements = map_fn(std::forward<Elements>(elements))), ...);
}

template <typename MapFn>
constexpr auto transform(MapFn&& map_fn)
{
    return [f = std::forward<MapFn>(map_fn)](auto&&... elements) {
        return transform(f, std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------- transform_reduce ----------------------------- */

template <typename T, typename MapFn, typename BinaryOp, typename... Elements>
constexpr auto transform_reduce(T init,
                                MapFn&& map_fn,
                                BinaryOp reduce_fn,
                                Elements&&... elements) -> T
{
    ((init = reduce_fn(init, map_fn(std::forward<Elements>(elements)))), ...);
    return init;
}

template <typename T>
constexpr auto transform_reduce(T init)
{
    return detail::Partial_application_three{
        [init = std::move(init)](auto&& map_fn, auto&& reduce_fn,
                                 auto&&... elements) {
            return transform_reduce(
                init, std::forward<decltype(map_fn)>(map_fn),
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename MapFn>
constexpr auto transform_reduce(T init, MapFn map_fn)
{
    return detail::Partial_application_two{
        [init = std::move(init), map_fn = std::move(map_fn)](
            auto&& reduce_fn, auto&&... elements) {
            return transform_reduce(
                init, map_fn, std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename MapFn, typename BinaryOp>
constexpr auto transform_reduce(T init, MapFn map_fn, BinaryOp reduce_fn)
{
    return detail::Partial_application_one{
        [init = std::move(init), map_fn = std::move(map_fn),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return transform_reduce(
                init, map_fn, reduce_fn,
                std::forward<decltype(elements)>(elements)...);
        }};
}

/* -------------------------------- count_if -------------------------------- */

template <typename UnaryPredicate, typename... Elements>
constexpr auto count_if(UnaryPredicate&& predicate, Elements&&... elements)
    -> std::size_t
{
    return reduce(
        0uL,
        [&predicate](auto count, auto const& x) {
            return predicate(x) ? count + 1 : count;
        },
        std::forward<Elements>(elements)...);
}

template <typename UnaryPredicate>
constexpr auto count_if(UnaryPredicate&& predicate)
{
    return [predicate =
                std::forward<UnaryPredicate>(predicate)](auto&&... elements) {
        return count_if(predicate,
                        std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- count ---------------------------------- */

template <typename T, typename... Elements>
constexpr auto count(T&& x, Elements&&... elements) -> std::size_t
{
    return count_if([&x](auto y) { return y == x; },
                    std::forward<Elements>(elements)...);
}

template <typename T>
constexpr auto count(T&& x)
{
    return [x = std::forward<T>(x)](auto&&... elements) {
        return count(x, std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- all_of --------------------------------- */
template <typename UnaryPredicate, typename... Elements>
constexpr auto all_of(UnaryPredicate&& predicate, Elements&&... elements)
    -> bool
{
    // TODO implement with transform_reduce(...)
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

/* ---------------------------------- get ----------------------------------- */

template <std::size_t I, typename... Elements>
auto get(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0)
        return;
    else {
        return std::get<I>(
            std::forward_as_tuple(std::forward<Elements>(elements)...));
    }
}

/* --------------------------------- first ---------------------------------- */

template <typename... Elements>
auto first(Elements&&... elements)
{
    return get<0>(std::forward<Elements>(elements)...);
}

/* --------------------------------- last ----------------------------------- */

template <typename... Elements>
auto last(Elements&&... elements)
{
    return get<sizeof...(Elements) - 1>(std::forward<Elements>(elements)...);
}

namespace reverse {

/* --------------------------- reverse::for_each ---------------------------- */
template <typename UnaryFunction, typename... Elements>
constexpr auto for_each(UnaryFunction&& func, Elements&&... elements) -> void
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

/* ---------------------------- reverse::all_of ----------------------------- */
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

/* ---------------------------- reverse::any_of ----------------------------- */
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

/* --------------------------- reverse::none_of ----------------------------- */
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

/* ---------------------------- reverse::reduce ----------------------------- */
template <typename T, typename BinaryOp, typename... Elements>
constexpr auto reduce(T init, BinaryOp&& binary_op, Elements&&... elements) -> T
{
    auto foo = 0;
    [[maybe_unused]] auto const i =
        (foo = ... = (init = binary_op(init, elements), 0));
    return init;
}

template <typename T>
constexpr auto reduce(T init)
{
    return detail::Partial_application_two{
        [init = std::move(init)](auto&& binary_op, auto&&... elements) {
            return reverse::reduce(
                init, std::forward<decltype(binary_op)>(binary_op),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename BinaryOp>
constexpr auto reduce(T init, BinaryOp binary_op)
{
    return detail::Partial_application_one{
        [init      = std::move(init),
         binary_op = std::move(binary_op)](auto&&... elements) {
            return reverse::reduce(
                init, binary_op, std::forward<decltype(elements)>(elements)...);
        }};
}

/* -------------------------- reverse::transform ---------------------------- */

template <typename MapFn, typename... Elements>
constexpr auto transform(MapFn&& map_fn, Elements&&... elements) -> void
{
    auto foo = 0;
    [[maybe_unused]] auto const i =
        (foo = ... = (elements = map_fn(std::forward<Elements>(elements)), 0));
}

template <typename MapFn>
constexpr auto transform(MapFn&& map_fn)
{
    return [f = std::forward<MapFn>(map_fn)](auto&&... elements) {
        return reverse::transform(
            f, std::forward<decltype(elements)>(elements)...);
    };
}

/* ---------------------- reverse::transform_reduce ------------------------- */

template <typename T, typename MapFn, typename BinaryOp, typename... Elements>
constexpr auto transform_reduce(T init,
                                MapFn&& map_fn,
                                BinaryOp reduce_fn,
                                Elements&&... elements) -> T
{
    auto x = 0;
    (x = ... = ((
         init = reduce_fn(init, map_fn(std::forward<Elements>(elements))), 0)));
    return init;
}

template <typename T>
constexpr auto transform_reduce(T init)
{
    return detail::Partial_application_three{
        [init = std::move(init)](auto&& map_fn, auto&& reduce_fn,
                                 auto&&... elements) {
            return reverse::transform_reduce(
                init, std::forward<decltype(map_fn)>(map_fn),
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename MapFn>
constexpr auto transform_reduce(T init, MapFn map_fn)
{
    return detail::Partial_application_two{
        [init = std::move(init), map_fn = std::move(map_fn)](
            auto&& reduce_fn, auto&&... elements) {
            return reverse::transform_reduce(
                init, map_fn, std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename MapFn, typename BinaryOp>
constexpr auto transform_reduce(T init, MapFn map_fn, BinaryOp reduce_fn)
{
    return detail::Partial_application_one{
        [init = std::move(init), map_fn = std::move(map_fn),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return reverse::transform_reduce(
                init, map_fn, reduce_fn,
                std::forward<decltype(elements)>(elements)...);
        }};
}

/* -------------------------------------------------------------------------- */

}  // namespace reverse
}  // namespace halg
#endif  // HALG_HPP
