#ifndef HAL_HPP
#define HAL_HPP
#include <concepts>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace hal {

// to_tuple(...) from:
// reddit.com/r/cpp/comments/4yp7fv/c17_structured_bindings_convert_struct_to_a_tuple
/* ------------------------------ to_tuple ---------------------------------- */

namespace detail {

template <typename Fn, typename... Args>
auto return_type_impl()
    -> decltype(std::declval<Fn>()(std::declval<Args>()...));

/// Is the return type of an object of \p Fn with \p Args... applied to it.
template <typename Fn, typename... Args>
using Return_t = decltype(return_type_impl<Fn, Args...>());

template <typename T, typename... Args>
auto test_is_braces_constructible(int)
    -> decltype(void(T{std::declval<Args>()...}), std::true_type{});

template <typename...>
auto test_is_braces_constructible(...) -> std::false_type;

template <typename T, typename... Args>
using is_braces_constructible =
    decltype(test_is_braces_constructible<T, Args...>(0));

template <typename T, typename... Args>
using has_members = is_braces_constructible<T, Args...>;

struct any_type {
    template <typename T>
    constexpr operator T();
};

template <typename Make_tup, typename T>
constexpr auto to_tuple_impl(Make_tup&& make_tup, T&& object)
{
    using namespace detail;
    using obj_t = std::decay_t<T>;
    using X     = any_type;
    if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X, X, X,
                              X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13, x14,
                x15] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12,
                        x13, x14, x15);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X, X,
                                   X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13,
                x14] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12,
                        x13, x14);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X, X,
                                   X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12, x13] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12,
                        x13);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X, X,
                                   X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11, x12);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X,
                                   X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10, x11);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9, x10);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8, x9] =
            std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8, x9);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7, x8] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7, x8);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6, x7] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6, x7);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5, x6] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5, x6);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4, X5] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4, X5);
    }
    else if constexpr (has_members<obj_t, X, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3, x4] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3, x4);
    }
    else if constexpr (has_members<obj_t, X, X, X, X>{}) {
        auto&& [x0, x1, x2, x3] = std::forward<T>(object);
        return make_tup(x0, x1, x2, x3);
    }
    else if constexpr (has_members<obj_t, X, X, X>{}) {
        auto&& [x0, x1, x2] = std::forward<T>(object);
        return make_tup(x0, x1, x2);
    }
    else if constexpr (has_members<obj_t, X, X>{}) {
        auto&& [x0, x1] = std::forward<T>(object);
        return make_tup(x0, x1);
    }
    else if constexpr (has_members<obj_t, X>{}) {
        auto&& [x0] = std::forward<T>(object);
        return make_tup(x0);
    }
    else
        return make_tup();
}

}  // namespace detail

template <typename T>
constexpr auto to_tuple(T&& object)
{
    return detail::to_tuple_impl(
        [](auto&&... x) {
            return std::make_tuple(std::forward<decltype(x)>(x)...);
        },
        std::forward<T>(object));
}

template <typename T>
constexpr auto to_ref_tuple(T&& object)
{
    return detail::to_tuple_impl(
        [](auto&&... x) {
            return std::forward_as_tuple(std::forward<decltype(x)>(x)...);
        },
        std::forward<T>(object));
}

// Modified from:
// cppreference.com/w/cpp/utility/make_from_tuple
/* ---------------------------- from_tuple ---------------------------------- */
namespace detail {
template <typename T, typename Tuple, std::size_t... I>
constexpr auto from_tuple_impl(Tuple&& t, std::index_sequence<I...>) -> T
{
    return T{std::get<I>(std::forward<Tuple>(t))...};
}
}  // namespace detail

// Uses braces instead of parentheses to construct, for working with structs
template <typename T, typename Tuple>
constexpr auto from_tuple(Tuple&& t) -> T
{
    return detail::from_tuple_impl<T>(
        std::forward<Tuple>(t),
        std::make_index_sequence<
            std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

/* ---------------------------Function Objects -------------------------------*/
namespace detail {

/* ------------------------------- Curried -----------------------------------*/
// Inspired by Functional Programming in C++ by Ivan Cukic, section 11.3.

/// Creates a Curried Function.
/** Wraps a function which captures arguments with the call operator until
    minimum_args is reached, and the function can be invoked with the captured
    arguments. Use std::reference_wrapper if you need captured arguments to be
    references. */
template <std::size_t minimum_args,
          typename Function,
          typename... Captured_args>
class Curried {
   public:
    // Needed because: like std::decay_t, but with std::reference_wrapper -> &
    using Captured_t =
        decltype(std::make_tuple(std::declval<Captured_args>()...));

   public:
    /// Create a Curried function with no captured arguments.
    constexpr explicit Curried(Function f) : function_{std::move(f)} {}

    /// Internal use only, can't be private because of templates being diff type
    constexpr Curried(Function f, Captured_t args)
        : function_{std::move(f)}, captured_{std::move(args)}
    {}

   public:
    /// Either capture the args or invoke the function and return the result.
    template <typename... New_args>
    constexpr auto operator()(New_args&&... args) const
    {
        constexpr auto arg_count =
            sizeof...(New_args) + sizeof...(Captured_args);

        if constexpr (arg_count >= minimum_args &&
                      std::invocable<Function, Captured_args..., New_args...>) {
            // If invoking the function, use references of the args...
            auto all_args = std::tuple_cat(
                captured_,
                std::forward_as_tuple(std::forward<New_args>(args)...));
            return std::apply(function_, all_args);
        }
        else {
            auto new_args = std::make_tuple(std::forward<New_args>(args)...);
            auto all_args = std::tuple_cat(captured_, std::move(new_args));
            return Curried<minimum_args, Function, Captured_args...,
                           New_args...>{function_, all_args};
        }
    }

   private:
    Function function_;
    Captured_t captured_;
};

/// Make a Curried function object.
template <std::size_t minimum_args, typename Function>
constexpr auto make_curried(Function&& f) -> Curried<minimum_args, Function>
{
    return Curried<minimum_args, Function>{std::forward<Function>(f)};
}
}  // namespace detail

/* -------------------------------- for_each -------------------------------- */
template <typename UnaryOp, typename... Elements>
    requires((std::invocable<UnaryOp, Elements> && ...))
constexpr auto for_each_impl(UnaryOp&& func, Elements&&... elements) -> void
{
    (func(std::forward<Elements>(elements)), ...);
}

inline auto constexpr for_each =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return for_each_impl(std::forward<decltype(a)>(a),
                             std::forward<decltype(b)>(b)...);
    });

namespace memberwise {
template <typename UnaryOp, typename Aggregate>
constexpr auto for_each_impl(UnaryOp&& func, Aggregate&& aggregate) -> void
{
    std::apply(hal::for_each(std::forward<UnaryOp>(func)),
               hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
}

inline auto constexpr for_each =
    hal::detail::make_curried<1>([](auto&& a, auto&& b) {
        return hal::memberwise::for_each_impl(std::forward<decltype(a)>(a),
                                              std::forward<decltype(b)>(b));
    });
}  // namespace memberwise

/* --------------------------------- reduce --------------------------------- */
template <typename T, typename BinaryOp, typename... Elements>
    requires((std::invocable<BinaryOp, T, Elements> && ...) &&
             (std::convertible_to<detail::Return_t<BinaryOp, T, Elements>, T> &&
              ...))
constexpr auto reduce_impl(T init, BinaryOp&& reduce_fn, Elements&&... elements)
    -> T
{
    ((init = reduce_fn(init, std::forward<Elements>(elements))), ...);
    return init;
}

inline auto constexpr reduce =
    detail::make_curried<3>([](auto&& a, auto&& b, auto&&... c) {
        return reduce_impl(std::forward<decltype(a)>(a),
                           std::forward<decltype(b)>(b),
                           std::forward<decltype(c)>(c)...);
    });

namespace memberwise {
template <typename T, typename BinaryOp, typename Aggregate>
constexpr auto reduce_impl(T init, BinaryOp&& reduce_fn, Aggregate&& aggregate)
    -> T
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size == 0)
        return init;
    else {
        return std::apply(
            hal::reduce(std::move(init), std::forward<BinaryOp>(reduce_fn)),
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}

inline auto constexpr reduce =
    hal::detail::make_curried<3>([](auto&& a, auto&& b, auto&& c) {
        return hal::memberwise::reduce_impl(std::forward<decltype(a)>(a),
                                            std::forward<decltype(b)>(b),
                                            std::forward<decltype(c)>(c));
    });
}  // namespace memberwise

/* ---------------------------- partial_reduce ------------------------------ */
template <typename T, typename BinaryOp, typename... Elements>
constexpr void partial_reduce_impl(T init,
                                   BinaryOp&& reduce_fn,
                                   Elements&&... elements)
{
    auto wrapped_reduce = [&reduce_fn](T const& sum, auto& element) constexpr
    {
        auto const result = reduce_fn(sum, element);
        element           = result;
        return result;
    };
    reduce_impl(std::move(init), wrapped_reduce,
                std::forward<Elements>(elements)...);
}

inline auto constexpr partial_reduce =
    detail::make_curried<3>([](auto&& a, auto&& b, auto&&... c) {
        return partial_reduce_impl(std::forward<decltype(a)>(a),
                                   std::forward<decltype(b)>(b),
                                   std::forward<decltype(c)>(c)...);
    });

namespace memberwise {
template <typename T, typename BinaryOp, typename Aggregate>
constexpr void partial_reduce_impl(T init,
                                   BinaryOp&& reduce_fn,
                                   Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        return std::apply(
            hal::partial_reduce(std::move(init),
                                std::forward<BinaryOp>(reduce_fn)),
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}
inline auto constexpr partial_reduce =
    hal::detail::make_curried<3>([](auto&& a, auto&& b, auto&& c) {
        return hal::memberwise::partial_reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c));
    });
}  // namespace memberwise

/* ------------------------------- transform -------------------------------- */
// output of calling the function is assignable to the current element
template <typename UnaryOp, typename... Elements>
    requires(
        (std::invocable<UnaryOp, Elements> && ...) &&
        (std::assignable_from<Elements, detail::Return_t<UnaryOp, Elements>> &&
         ...) &&
        (!std::is_rvalue_reference_v<Elements> && ...))
constexpr auto transform_impl(UnaryOp&& transform_fn, Elements&&... elements)
    -> void
{
    ((elements = transform_fn(std::forward<Elements>(elements))), ...);
}

inline auto constexpr transform =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return transform_impl(std::forward<decltype(a)>(a),
                              std::forward<decltype(b)>(b)...);
    });

/* --------------------------- transform_reduce ----------------------------- */

template <typename T, typename UnaryOp, typename BinaryOp, typename... Elements>
    // clang-format off
    requires(
        (std::invocable<UnaryOp, Elements> && ...) &&
        (std::invocable<BinaryOp, T&, detail::Return_t<UnaryOp, Elements>> && ...) &&
        (std::assignable_from<T&, detail::Return_t<BinaryOp, T&, detail::Return_t<UnaryOp, Elements>>> && ...))
// clang-format on
constexpr auto transform_reduce_impl(T init,
                                     UnaryOp&& transform_fn,
                                     BinaryOp&& reduce_fn,
                                     Elements&&... elements) -> T
{
    ((init = reduce_fn(init, transform_fn(std::forward<Elements>(elements)))),
     ...);
    return init;
}

inline auto constexpr transform_reduce =
    detail::make_curried<4>([](auto&& a, auto&& b, auto&& c, auto&&... d) {
        return transform_reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c), std::forward<decltype(d)>(d)...);
    });

/* ------------------------ partial_transform_reduce ------------------------ */
template <typename T, typename UnaryOp, typename BinaryOp, typename... Elements>
    // clang-format off
    requires(
        (std::invocable<UnaryOp, Elements> && ...) &&
        (std::invocable<BinaryOp, T&, detail::Return_t<UnaryOp, Elements>> && ...) &&
        (std::assignable_from<T&, detail::Return_t<BinaryOp, T&, detail::Return_t<UnaryOp, Elements>>> && ...) &&
        (std::assignable_from<Elements, T> && ...) &&
        (!std::is_rvalue_reference_v<Elements> && ...))
// clang-format on
constexpr void partial_transform_reduce_impl([[maybe_unused]] T init,
                                             UnaryOp&& transform_fn,
                                             BinaryOp&& reduce_fn,
                                             Elements&&... elements)
{
    ((std::forward<Elements>(elements) = init =
          reduce_fn(init, transform_fn(std::forward<Elements>(elements)))),
     ...);
}

inline auto constexpr partial_transform_reduce =
    detail::make_curried<4>([](auto&& a, auto&& b, auto&& c, auto&&... d) {
        return partial_transform_reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c), std::forward<decltype(d)>(d)...);
    });

/* -------------------------------- find_if --------------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto find_if_impl(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    auto increment_until_true = [still_going = true](std::size_t count,
                                                     auto boolean) mutable {
        return still_going && !boolean ? count + 1
                                       : (still_going = false, count);
    };
    return transform_reduce_impl(0uL, std::forward<UnaryOp>(predicate),
                                 increment_until_true,
                                 std::forward<Elements>(elements)...);
}

inline auto constexpr find_if =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return find_if_impl(std::forward<decltype(a)>(a),
                            std::forward<decltype(b)>(b)...);
    });

/* ------------------------------ find_if_not ------------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto find_if_not_impl(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    return find_if_impl(std::not_fn(std::forward<UnaryOp>(predicate)),
                        std::forward<Elements>(elements)...);
}

inline auto constexpr find_if_not =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return find_if_not_impl(std::forward<decltype(a)>(a),
                                std::forward<decltype(b)>(b)...);
    });

/* --------------------------------- find ----------------------------------- */

template <typename T, typename... Elements>
constexpr auto find_impl(T&& x, Elements&&... elements) -> std::size_t
{
    auto equal_to_x = [&](auto y) { return y == x; };
    return find_if_impl(equal_to_x, std::forward<Elements>(elements)...);
}

inline auto constexpr find = detail::make_curried<2>([](auto&& a, auto&&... b) {
    return find_impl(std::forward<decltype(a)>(a),
                     std::forward<decltype(b)>(b)...);
});

/* -------------------------------- count_if -------------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto count_if_impl(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    return reduce_impl(
        0uL,
        [&predicate](auto count, auto const& x) {
            return predicate(x) ? count + 1 : count;
        },
        std::forward<Elements>(elements)...);
}

inline auto constexpr count_if =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return count_if_impl(std::forward<decltype(a)>(a),
                             std::forward<decltype(b)>(b)...);
    });

/* --------------------------------- count ---------------------------------- */

template <typename T, typename... Elements>
constexpr auto count_impl(T&& x, Elements&&... elements) -> std::size_t
{
    return count_if_impl([&x](auto y) { return y == x; },
                         std::forward<Elements>(elements)...);
}

inline auto constexpr count =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return count_impl(std::forward<decltype(a)>(a),
                          std::forward<decltype(b)>(b)...);
    });

/* --------------------------------- all_of --------------------------------- */
template <typename UnaryOp, typename... Elements>
    requires((std::predicate<UnaryOp, Elements> && ...))
constexpr auto all_of_impl(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return (predicate(std::forward<Elements>(elements)) && ...);
}

inline auto constexpr all_of =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return all_of_impl(std::forward<decltype(a)>(a),
                           std::forward<decltype(b)>(b)...);
    });

template <typename... Elements>
constexpr auto all(Elements&&... elements) -> bool
{
    return all_of_impl(std::identity{}, std::forward<Elements>(elements)...);
}

/* --------------------------------- any_of --------------------------------- */
template <typename UnaryOp, typename... Elements>
    requires((std::predicate<UnaryOp, Elements> && ...))
constexpr auto any_of_impl(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return (predicate(std::forward<Elements>(elements)) || ...);
}

inline auto constexpr any_of =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return any_of_impl(std::forward<decltype(a)>(a),
                           std::forward<decltype(b)>(b)...);
    });

template <typename... Elements>
constexpr auto any(Elements&&... elements) -> bool
{
    return any_of_impl(std::identity{}, std::forward<Elements>(elements)...);
}

/* -------------------------------- none_of --------------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto none_of_impl(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return !any_of_impl(std::forward<UnaryOp>(predicate),
                        std::forward<Elements>(elements)...);
}

inline auto constexpr none_of =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return none_of_impl(std::forward<decltype(a)>(a),
                            std::forward<decltype(b)>(b)...);
    });

template <typename... Elements>
constexpr auto none(Elements&&... elements) -> bool
{
    return none_of_impl(std::identity{}, std::forward<Elements>(elements)...);
}

/* ----------------------- adjacent_transform_reduce ------------------------ */

template <typename T,
          typename BinaryOp_1,
          typename BinaryOp_2,
          typename L,
          typename R,
          typename... Tail>
constexpr auto adjacent_transform_reduce_impl(T init,
                                              BinaryOp_1&& transform_fn,
                                              BinaryOp_2&& reduce_fn,
                                              L&& left,
                                              R&& right,
                                              Tail&&... tail) -> T
{
    auto transformed = transform_fn(std::forward<L>(left), right);
    auto reduced     = reduce_fn(std::move(init), std::move(transformed));
    if constexpr (sizeof...(Tail) == 0)
        return reduced;
    else {
        return adjacent_transform_reduce_impl(std::move(reduced), transform_fn,
                                              reduce_fn, right,
                                              std::forward<Tail>(tail)...);
    }
}

inline auto constexpr adjacent_transform_reduce = detail::make_curried<5>(
    [](auto&& a, auto&& b, auto&& c, auto&& d, auto&& e, auto&&... f) {
        return adjacent_transform_reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c), std::forward<decltype(d)>(d),
            std::forward<decltype(e)>(e), std::forward<decltype(f)>(f)...);
    });

/* --------------------------- adjacent_transform --------------------------- */
template <typename BinaryOp, typename L, typename R, typename... Tail>
constexpr void adjacent_transform_impl(BinaryOp&& transform_fn,
                                       L&& left,
                                       R&& right,
                                       Tail&&... tail)
{
    left = transform_fn(std::forward<L>(left), right);
    if constexpr (sizeof...(Tail) == 0)
        return;
    else {
        return adjacent_transform_impl(transform_fn, right,
                                       std::forward<Tail>(tail)...);
    }
}

inline auto constexpr adjacent_transform =
    detail::make_curried<3>([](auto&& a, auto&& b, auto&& c, auto&&... d) {
        return adjacent_transform_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c), std::forward<decltype(d)>(d)...);
    });

/* -------------------------- adjacent_difference --------------------------- */
template <typename... Elements>
constexpr void adjacent_difference(Elements&&... elements)
{
    adjacent_transform_impl([](auto const& l, auto const& r) { return r - l; },
                            std::forward<Elements>(elements)...);
}

/* ----------------------------- adjacent_find ------------------------------ */

template <typename... Elements>
constexpr auto adjacent_find(Elements&&... elements) -> std::size_t
{
    auto increment_until_true = [still_going = true](std::size_t count,
                                                     bool found) mutable {
        return still_going && !found ? count + 1 : (still_going = false, count);
    };
    return adjacent_transform_reduce_impl(0uL, std::equal_to<>{},
                                          increment_until_true,
                                          std::forward<Elements>(elements)...);
}

/* ---------------------------------- get ----------------------------------- */

template <std::size_t I, typename... Elements>
constexpr auto get(Elements&&... elements) -> decltype(auto)
{
    static_assert(I < sizeof...(Elements),
                  "Cannot access elements outside of parameter pack");
    return std::get<I>(
        std::forward_as_tuple(std::forward<Elements>(elements)...));
}

/* --------------------------------- first ---------------------------------- */

template <typename... Elements>
constexpr auto first(Elements&&... elements) -> decltype(auto)
{
    return hal::get<0>(std::forward<Elements>(elements)...);
}

/* --------------------------------- last ----------------------------------- */

template <typename... Elements>
constexpr auto last(Elements&&... elements) -> decltype(auto)
{
    return hal::get<sizeof...(Elements) - 1>(
        std::forward<Elements>(elements)...);
}

/* ------------------------------ partial_sum ------------------------------- */

template <typename... Elements>
constexpr void partial_sum(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0uL)
        return;
    using reduce_t =
        std::decay_t<decltype(hal::first(std::forward<Elements>(elements)...))>;
    partial_reduce(reduce_t(0),
                   std::plus<>{})(std::forward<Elements>(elements)...);
}

namespace memberwise {
template <typename Aggregate>
constexpr void partial_sum(Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        std::apply(
            [](auto&&... elements) {
                hal::partial_sum(std::forward<decltype(elements)>(elements)...);
            },
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}
}  // namespace memberwise

/* -------------------------- partial_difference ---------------------------- */

template <typename... Elements>
constexpr void partial_difference(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0uL)
        return;
    using reduce_t =
        std::decay_t<decltype(hal::first(std::forward<Elements>(elements)...))>;
    partial_reduce(reduce_t(0),
                   std::minus<>{})(std::forward<Elements>(elements)...);
}

namespace memberwise {
template <typename Aggregate>
constexpr void partial_difference(Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        std::apply(
            [](auto&&... elements) {
                hal::partial_difference(
                    std::forward<decltype(elements)>(elements)...);
            },
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}
}  // namespace memberwise

/* --------------------------- partial_product ------------------------------ */

template <typename... Elements>
constexpr void partial_product(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0uL)
        return;
    using reduce_t =
        std::decay_t<decltype(hal::first(std::forward<Elements>(elements)...))>;
    partial_reduce(reduce_t(1),
                   std::multiplies<>{})(std::forward<Elements>(elements)...);
}

namespace memberwise {
template <typename Aggregate>
constexpr void partial_product(Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        std::apply(
            [](auto&&... elements) {
                hal::partial_product(
                    std::forward<decltype(elements)>(elements)...);
            },
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}
}  // namespace memberwise

/* --------------------------- partial_quotient ----------------------------- */

template <typename... Elements>
constexpr void partial_quotient(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0uL)
        return;
    using reduce_t =
        std::decay_t<decltype(hal::first(std::forward<Elements>(elements)...))>;
    partial_reduce(reduce_t(1),
                   std::divides<>{})(std::forward<Elements>(elements)...);
}

namespace memberwise {
template <typename Aggregate>
constexpr void partial_quotient(Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        std::apply(
            [](auto&&... elements) {
                hal::partial_quotient(
                    std::forward<decltype(elements)>(elements)...);
            },
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}
}  // namespace memberwise

/* --------------------------------------------------------------------------
   ------------------------------ REVERSE -----------------------------------
   -------------------------------------------------------------------------- */

namespace reverse {

// TODO continue from here.

/* --------------------------- reverse::for_each ---------------------------- */
template <typename UnaryOp, typename... Elements>
    requires((std::invocable<UnaryOp, Elements> && ...))
constexpr auto for_each_impl(UnaryOp&& func, Elements&&... elements) -> void
{
    // clang 11.0.0 warning on unused expression result without void cast.
    [[maybe_unused]] auto foo = 0;
    (void)(foo = ... = (func(elements), 0));
}

inline auto constexpr for_each =
    detail::make_curried<2>([](auto&& a, auto&&... b) {
        return reverse::for_each_impl(std::forward<decltype(a)>(a),
                                      std::forward<decltype(b)>(b)...);
    });

namespace memberwise {
template <typename UnaryOp, typename Aggregate>
constexpr auto for_each_impl(UnaryOp&& func, Aggregate&& aggregate) -> void
{
    std::apply(hal::reverse::for_each(std::forward<UnaryOp>(func)),
               hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
}

inline auto constexpr for_each =
    hal::detail::make_curried<1>([](auto&& a, auto&& b) {
        return hal::reverse::memberwise::for_each_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b));
    });

}  // namespace memberwise

/* ---------------------------- reverse::all_of ----------------------------- */
template <typename UnaryOp>
constexpr auto all_of_impl(UnaryOp&&) -> bool
{
    return true;
}

template <typename UnaryOp, typename Head, typename... Tail>
constexpr auto all_of_impl(UnaryOp&& predicate, Head&& head, Tail&&... tail)
    -> bool
{
    return all_of_impl(predicate, std::forward<Tail>(tail)...) &&
           predicate(head);
}

inline auto constexpr all_of =
    hal::detail::make_curried<2>([](auto&& a, auto&&... b) {
        return hal::reverse::all_of_impl(std::forward<decltype(a)>(a),
                                         std::forward<decltype(b)>(b)...);
    });

template <typename... Elements>
constexpr auto all(Elements&&... elements) -> bool
{
    return hal::reverse::all_of_impl(std::identity{},
                                     std::forward<Elements>(elements)...);
}

/* ---------------------------- reverse::any_of ----------------------------- */
template <typename UnaryOp>
constexpr auto any_of_impl(UnaryOp&&) -> bool
{
    return false;
}

template <typename UnaryOp, typename Head, typename... Tail>
constexpr auto any_of_impl(UnaryOp&& predicate, Head&& head, Tail&&... tail)
    -> bool
{
    return any_of_impl(predicate, std::forward<Tail>(tail)...) ||
           predicate(head);
}

inline auto constexpr any_of =
    hal::detail::make_curried<2>([](auto&& a, auto&&... b) {
        return hal::reverse::any_of_impl(std::forward<decltype(a)>(a),
                                         std::forward<decltype(b)>(b)...);
    });

template <typename... Elements>
constexpr auto any(Elements&&... elements) -> bool
{
    return hal::reverse::any_of_impl(std::identity{},
                                     std::forward<Elements>(elements)...);
}

/* --------------------------- reverse::none_of ----------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto none_of_impl(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return !reverse::any_of_impl(std::forward<UnaryOp>(predicate),
                                 std::forward<Elements>(elements)...);
}

inline auto constexpr none_of =
    hal::detail::make_curried<2>([](auto&& a, auto&&... b) {
        return hal::reverse::none_of_impl(std::forward<decltype(a)>(a),
                                          std::forward<decltype(b)>(b)...);
    });

template <typename... Elements>
constexpr auto none(Elements&&... elements) -> bool
{
    return hal::reverse::none_of_impl(std::identity{},
                                      std::forward<Elements>(elements)...);
}

/* ---------------------------- reverse::reduce ----------------------------- */
template <typename T, typename BinaryOp>
constexpr auto reduce_impl(T init, BinaryOp&&) -> T
{
    return init;
}

// Base case of zero elements is ambiguous with partial application, new name.
template <typename T, typename BinaryOp, typename Head, typename... Tail>
constexpr auto reduce_impl(T init,
                           BinaryOp&& reduce_fn,
                           Head&& head,
                           Tail&&... tail) -> T
{
    return reduce_fn(
        reduce_impl(std::move(init), reduce_fn, std::forward<Tail>(tail)...),
        std::forward<Head>(head));
}

inline auto constexpr reduce =
    hal::detail::make_curried<3>([](auto&& a, auto&& b, auto&&... c) {
        return hal::reverse::reduce_impl(std::forward<decltype(a)>(a),
                                         std::forward<decltype(b)>(b),
                                         std::forward<decltype(c)>(c)...);
    });

namespace memberwise {
template <typename T, typename BinaryOp, typename Aggregate>
constexpr auto reduce_impl(T init, BinaryOp&& reduce_fn, Aggregate&& aggregate)
    -> T
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size == 0)
        return init;
    else {
        return std::apply(
            hal::reverse::reduce(std::move(init),
                                 std::forward<BinaryOp>(reduce_fn)),
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}

inline auto constexpr reduce =
    hal::detail::make_curried<3>([](auto&& a, auto&& b, auto&& c) {
        return hal::reverse::memberwise::reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c));
    });
}  // namespace memberwise

/* ----------------------- reverse::partial_reduce -------------------------- */
template <typename T, typename BinaryOp, typename... Elements>
constexpr void partial_reduce_impl(T init,
                                   BinaryOp&& reduce_fn,
                                   Elements&&... elements)
{
    auto wrapped_reduce = [&reduce_fn](T const& sum, auto& element) constexpr
    {
        auto const result = reduce_fn(sum, element);
        element           = result;
        return result;
    };
    reverse::reduce_impl(init, wrapped_reduce,
                         std::forward<Elements>(elements)...);
}

inline auto constexpr partial_reduce =
    hal::detail::make_curried<3>([](auto&& a, auto&& b, auto&&... c) {
        return hal::reverse::partial_reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c)...);
    });

namespace memberwise {
template <typename T, typename BinaryOp, typename Aggregate>
constexpr void partial_reduce_impl(T init,
                                   BinaryOp&& reduce_fn,
                                   Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        return std::apply(
            hal::reverse::partial_reduce(std::move(init),
                                         std::forward<BinaryOp>(reduce_fn)),
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}

inline auto constexpr partial_reduce =
    hal::detail::make_curried<3>([](auto&& a, auto&& b, auto&& c) {
        return hal::reverse::memberwise::partial_reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c));
    });
}  // namespace memberwise

/* -------------------------- reverse::transform ---------------------------- */

template <typename UnaryOp, typename... Elements>
    requires(
        (std::invocable<UnaryOp, Elements> && ...) &&
        (std::assignable_from<Elements, detail::Return_t<UnaryOp, Elements>> &&
         ...) &&
        (!std::is_rvalue_reference_v<Elements> && ...))
constexpr auto transform_impl(UnaryOp&& transform_fn, Elements&&... elements)
    -> void
{
    // clang 11.0.0 warning on unused expression result without void cast.
    [[maybe_unused]] auto foo = 0;
    (void)(foo = ... =
               (elements = transform_fn(std::forward<Elements>(elements)), 0));
}

inline auto constexpr transform =
    hal::detail::make_curried<2>([](auto&& op, auto&&... e) {
        return hal::reverse::transform_impl(std::forward<decltype(op)>(op),
                                            std::forward<decltype(e)>(e)...);
    });

/* ---------------------- reverse::transform_reduce ------------------------- */

template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto transform_reduce_impl(T init, UnaryOp&&, BinaryOp&&) -> T
{
    return init;
}

// Base case of zero elements is ambiguous with partial application, new name.
template <typename T,
          typename UnaryOp,
          typename BinaryOp,
          typename Head,
          typename... Tail>
constexpr auto transform_reduce_impl(T init,
                                     UnaryOp&& transform_fn,
                                     BinaryOp&& reduce_fn,
                                     Head&& head,
                                     Tail&&... tail) -> T
{
    auto tail_result = transform_reduce_impl(
        std::move(init), transform_fn, reduce_fn, std::forward<Tail>(tail)...);
    auto head_result = transform_fn(head);
    return reduce_fn(std::move(tail_result), std::move(head_result));
}

inline auto constexpr transform_reduce =
    hal::detail::make_curried<4>([](auto&& a, auto&& b, auto&& c, auto&&... d) {
        return hal::reverse::transform_reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c), std::forward<decltype(d)>(d)...);
    });

/* ------------------- reverse::partial_transform_reduce -------------------- */
template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto partial_transform_reduce_impl(T init, UnaryOp&&, BinaryOp&&) -> T
{
    return init;
}

// Base case of zero elements is ambiguous with partial application, new name.
template <typename T,
          typename UnaryOp,
          typename BinaryOp,
          typename Head,
          typename... Tail>
constexpr auto partial_transform_reduce_impl(T init,
                                             UnaryOp&& transform_fn,
                                             BinaryOp&& reduce_fn,
                                             Head&& head,
                                             Tail&&... tail) -> T
{
    auto tail_result = partial_transform_reduce_impl(
        std::move(init), transform_fn, reduce_fn, std::forward<Tail>(tail)...);
    auto head_result = transform_fn(head);
    std::forward<Head>(head) =
        reduce_fn(std::move(tail_result), std::move(head_result));
    return head;
}

inline auto constexpr partial_transform_reduce =
    hal::detail::make_curried<4>([](auto&& a, auto&& b, auto&& c, auto&&... d) {
        return hal::reverse::partial_transform_reduce_impl(
            std::forward<decltype(a)>(a), std::forward<decltype(b)>(b),
            std::forward<decltype(c)>(c), std::forward<decltype(d)>(d)...);
    });

/* --------------------------- reverse::find_if ----------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto find_if_impl(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    auto decrement_until_true =
        [still_going = true](std::size_t count, bool predicate_result) mutable {
            return still_going && !predicate_result
                       ? count - 1
                       : (still_going = false, count);
        };

    auto const result = reverse::transform_reduce_impl(
        sizeof...(Elements) - 1, std::forward<UnaryOp>(predicate),
        decrement_until_true, std::forward<Elements>(elements)...);

    return result == -1uL ? sizeof...(Elements) : result;
}

inline auto constexpr find_if =
    hal::detail::make_curried<2>([](auto&& a, auto&&... b) {
        return hal::reverse::find_if_impl(std::forward<decltype(a)>(a),
                                          std::forward<decltype(b)>(b)...);
    });

/* ------------------------- reverse::find_if_not --------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto find_if_not_impl(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    return reverse::find_if_impl(std::not_fn(std::forward<UnaryOp>(predicate)),
                                 std::forward<Elements>(elements)...);
}

inline auto constexpr find_if_not =
    hal::detail::make_curried<2>([](auto&& a, auto&&... b) {
        return hal::reverse::find_if_not_impl(std::forward<decltype(a)>(a),
                                              std::forward<decltype(b)>(b)...);
    });

/* ---------------------------- reverse::find ------------------------------- */

template <typename T, typename... Elements>
constexpr auto find_impl(T&& x, Elements&&... elements) -> std::size_t
{
    auto equal_to_x = [&](auto y) { return y == x; };
    return reverse::find_if_impl(equal_to_x,
                                 std::forward<Elements>(elements)...);
}

inline auto constexpr find =
    hal::detail::make_curried<2>([](auto&& a, auto&&... b) {
        return hal::reverse::find_impl(std::forward<decltype(a)>(a),
                                       std::forward<decltype(b)>(b)...);
    });

/* ------------------------- reverse::partial_sum --------------------------- */

template <typename... Elements>
constexpr void partial_sum(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0uL)
        return;
    using reduce_t =
        std::decay_t<decltype(hal::last(std::forward<Elements>(elements)...))>;
    reverse::partial_reduce(reduce_t(0),
                            std::plus<>{})(std::forward<Elements>(elements)...);
}

namespace memberwise {
template <typename Aggregate>
constexpr void partial_sum(Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        std::apply(
            [](auto&&... elements) {
                hal::reverse::partial_sum(
                    std::forward<decltype(elements)>(elements)...);
            },
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}
}  // namespace memberwise

/* --------------------- reverse::partial_difference ------------------------ */

template <typename... Elements>
constexpr void partial_difference(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0uL)
        return;
    using reduce_t =
        std::decay_t<decltype(hal::last(std::forward<Elements>(elements)...))>;
    reverse::partial_reduce(
        reduce_t(0), std::minus<>{})(std::forward<Elements>(elements)...);
}

namespace memberwise {
template <typename Aggregate>
constexpr void partial_difference(Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        std::apply(
            [](auto&&... elements) {
                hal::reverse::partial_difference(
                    std::forward<decltype(elements)>(elements)...);
            },
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}
}  // namespace memberwise

/* ---------------------- reverse::partial_product -------------------------- */

template <typename... Elements>
constexpr void partial_product(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0uL)
        return;
    using reduce_t =
        std::decay_t<decltype(hal::last(std::forward<Elements>(elements)...))>;
    reverse::partial_reduce(
        reduce_t(1), std::multiplies<>{})(std::forward<Elements>(elements)...);
}

namespace memberwise {
template <typename Aggregate>
constexpr void partial_product(Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        std::apply(
            [](auto&&... elements) {
                hal::reverse::partial_product(
                    std::forward<decltype(elements)>(elements)...);
            },
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}
}  // namespace memberwise

/* ---------------------- reverse::partial_quotient ------------------------- */

template <typename... Elements>
constexpr void partial_quotient(Elements&&... elements)
{
    if constexpr (sizeof...(Elements) == 0uL)
        return;
    using reduce_t =
        std::decay_t<decltype(hal::last(std::forward<Elements>(elements)...))>;
    reverse::partial_reduce(
        reduce_t(1), std::divides<>{})(std::forward<Elements>(elements)...);
}

namespace memberwise {
template <typename Aggregate>
constexpr void partial_quotient(Aggregate&& aggregate)
{
    constexpr auto size =
        std::tuple_size_v<decltype(hal::to_ref_tuple(aggregate))>;
    if constexpr (size != 0) {
        std::apply(
            [](auto&&... elements) {
                hal::reverse::partial_quotient(
                    std::forward<decltype(elements)>(elements)...);
            },
            hal::to_ref_tuple(std::forward<Aggregate>(aggregate)));
    }
}

}  // namespace memberwise
}  // namespace reverse
}  // namespace hal
#endif  // HAL_HPP
