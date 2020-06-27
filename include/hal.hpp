#ifndef HAL_HPP
#define HAL_HPP
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

namespace hal {

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
template <typename UnaryOp, typename... Elements>
constexpr auto for_each(UnaryOp&& func, Elements&&... elements) -> void
{
    (func(std::forward<Elements>(elements)), ...);
}

template <typename UnaryOp>
constexpr auto for_each(UnaryOp&& func)
{
    return [f = std::forward<UnaryOp>(func)](auto&&... elements) {
        return for_each(f, std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- reduce --------------------------------- */
template <typename T, typename BinaryOp, typename... Elements>
constexpr auto reduce(T init, BinaryOp&& reduce_fn, Elements&&... elements) -> T
{
    ((init = reduce_fn(init, std::forward<Elements>(elements))), ...);
    return init;
}

template <typename T>
constexpr auto reduce(T init)
{
    return detail::Partial_application_two{
        [init = std::move(init)](auto&& reduce_fn, auto&&... elements) {
            return reduce(init, std::forward<decltype(reduce_fn)>(reduce_fn),
                          std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename BinaryOp>
constexpr auto reduce(T init, BinaryOp reduce_fn)
{
    return detail::Partial_application_one{
        [init      = std::move(init),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return reduce(init, reduce_fn,
                          std::forward<decltype(elements)>(elements)...);
        }};
}

/* ---------------------------- partial_reduce ------------------------------ */
template <typename T, typename BinaryOp, typename... Elements>
constexpr void partial_reduce(T init,
                              BinaryOp&& reduce_fn,
                              Elements&&... elements)
{
    auto wrapped_reduce = [&reduce_fn](T const& sum, auto& element) constexpr
    {
        auto const result = reduce_fn(sum, element);
        element           = result;
        return result;
    };
    reduce(std::move(init), wrapped_reduce,
           std::forward<Elements>(elements)...);
}

template <typename T>
constexpr auto partial_reduce(T init)
{
    return detail::Partial_application_two{
        [init = std::move(init)](auto&& reduce_fn, auto&&... elements) {
            return partial_reduce(
                init, std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename BinaryOp>
constexpr auto partial_reduce(T init, BinaryOp reduce_fn)
{
    return detail::Partial_application_one{
        [init      = std::move(init),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return partial_reduce(
                init, reduce_fn, std::forward<decltype(elements)>(elements)...);
        }};
}

/* ------------------------------- transform -------------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto transform(UnaryOp&& transform_fn, Elements&&... elements) -> void
{
    ((elements = transform_fn(std::forward<Elements>(elements))), ...);
}

template <typename UnaryOp>
constexpr auto transform(UnaryOp transform_fn)
{
    return [f = std::move(transform_fn)](auto&&... elements) {
        return transform(f, std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------- transform_reduce ----------------------------- */

template <typename T, typename UnaryOp, typename BinaryOp, typename... Elements>
constexpr auto transform_reduce(T init,
                                UnaryOp&& transform_fn,
                                BinaryOp&& reduce_fn,
                                Elements&&... elements) -> T
{
    ((init = reduce_fn(init, transform_fn(std::forward<Elements>(elements)))),
     ...);
    return init;
}

template <typename T>
constexpr auto transform_reduce(T init)
{
    return detail::Partial_application_three{
        [init = std::move(init)](auto&& transform_fn, auto&& reduce_fn,
                                 auto&&... elements) {
            return transform_reduce(
                init, std::forward<decltype(transform_fn)>(transform_fn),
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp>
constexpr auto transform_reduce(T init, UnaryOp transform_fn)
{
    return detail::Partial_application_two{
        [init = std::move(init), transform_fn = std::move(transform_fn)](
            auto&& reduce_fn, auto&&... elements) {
            return transform_reduce(
                init, transform_fn,
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto transform_reduce(T init,
                                UnaryOp transform_fn,
                                BinaryOp reduce_fn)
{
    return detail::Partial_application_one{
        [init = std::move(init), transform_fn = std::move(transform_fn),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return transform_reduce(
                init, transform_fn, reduce_fn,
                std::forward<decltype(elements)>(elements)...);
        }};
}

/* ------------------------ partial_transform_reduce ------------------------ */
template <typename T, typename UnaryOp, typename BinaryOp, typename... Elements>
constexpr void partial_transform_reduce(T init,
                                        UnaryOp&& transform_fn,
                                        BinaryOp&& reduce_fn,
                                        Elements&&... elements)
{
    ((std::forward<Elements>(elements) = init =
          reduce_fn(init, transform_fn(std::forward<Elements>(elements)))),
     ...);
}

template <typename T>
constexpr auto partial_transform_reduce(T init)
{
    return detail::Partial_application_three{
        [init = std::move(init)](auto&& transform_fn, auto&& reduce_fn,
                                 auto&&... elements) {
            return partial_transform_reduce(
                init, std::forward<decltype(transform_fn)>(transform_fn),
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp>
constexpr auto partial_transform_reduce(T init, UnaryOp transform_fn)
{
    return detail::Partial_application_two{
        [init = std::move(init), transform_fn = std::move(transform_fn)](
            auto&& reduce_fn, auto&&... elements) {
            return partial_transform_reduce(
                init, transform_fn,
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto partial_transform_reduce(T init,
                                        UnaryOp transform_fn,
                                        BinaryOp reduce_fn)
{
    return detail::Partial_application_one{
        [init = std::move(init), transform_fn = std::move(transform_fn),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return partial_transform_reduce(
                init, transform_fn, reduce_fn,
                std::forward<decltype(elements)>(elements)...);
        }};
}

/* -------------------------------- find_if --------------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto find_if(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    auto increment_until_true = [still_going = true](std::size_t count,
                                                     auto boolean) mutable {
        return still_going && !boolean ? count + 1
                                       : (still_going = false, count);
    };
    return transform_reduce(0uL, std::forward<UnaryOp>(predicate),
                            increment_until_true,
                            std::forward<Elements>(elements)...);
}

template <typename UnaryOp>
constexpr auto find_if(UnaryOp predicate)
{
    return [predicate = std::move(predicate)](auto&&... elements) {
        return find_if(predicate,
                       std::forward<decltype(elements)>(elements)...);
    };
}

/* ------------------------------ find_if_not ------------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto find_if_not(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    // std::not_fn is not constexpr, can't put this in its own function, not
    // constexpr, but it's allowed here for some reason.
    constexpr auto not_fn = [](auto&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&&... x) {
            return !f(std::forward<decltype(x)>(x)...);
        };
    };
    return find_if(not_fn(std::forward<UnaryOp>(predicate)),
                   std::forward<Elements>(elements)...);
}

template <typename UnaryOp>
constexpr auto find_if_not(UnaryOp&& predicate)
{
    return [predicate = std::forward<UnaryOp>(predicate)](auto&&... elements) {
        return find_if_not(predicate,
                           std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- find ----------------------------------- */

template <typename T, typename... Elements>
constexpr auto find(T&& x, Elements&&... elements) -> std::size_t
{
    auto equal_to_x = [&](auto y) { return y == x; };
    return find_if(equal_to_x, std::forward<Elements>(elements)...);
}

template <typename T>
constexpr auto find(T x)
{
    return [x = std::move(x)](auto&&... elements) {
        return find(x, std::forward<decltype(elements)>(elements)...);
    };
}

/* -------------------------------- count_if -------------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto count_if(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    return reduce(
        0uL,
        [&predicate](auto count, auto const& x) {
            return predicate(x) ? count + 1 : count;
        },
        std::forward<Elements>(elements)...);
}

template <typename UnaryOp>
constexpr auto count_if(UnaryOp predicate)
{
    return [predicate = std::move(predicate)](auto&&... elements) {
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
constexpr auto count(T x)
{
    return [x = std::move(x)](auto&&... elements) {
        return count(x, std::forward<decltype(elements)>(elements)...);
    };
}

/* --------------------------------- all_of --------------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto all_of(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return (predicate(std::forward<Elements>(elements)) && ...);
}

template <typename UnaryOp>
constexpr auto all_of(UnaryOp predicate)
{
    return [f = std::move(predicate)](auto&&... elements) {
        return all_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto all(Elements&&... elements) -> bool
{
    return all_of(detail::Identity{}, std::forward<Elements>(elements)...);
}

/* --------------------------------- any_of --------------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto any_of(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return (predicate(std::forward<Elements>(elements)) || ...);
}

template <typename UnaryOp>
constexpr auto any_of(UnaryOp predicate)
{
    return [f = std::move(predicate)](auto&&... elements) {
        return any_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto any(Elements&&... elements) -> bool
{
    return any_of(detail::Identity{}, std::forward<Elements>(elements)...);
}

/* -------------------------------- none_of --------------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto none_of(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return !any_of(std::forward<UnaryOp>(predicate),
                   std::forward<Elements>(elements)...);
}

template <typename UnaryOp>
constexpr auto none_of(UnaryOp predicate)
{
    return [f = std::move(predicate)](auto&&... elements) {
        return none_of(f, std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto none(Elements&&... elements) -> bool
{
    return none_of(detail::Identity{}, std::forward<Elements>(elements)...);
}

/* ----------------------- adjacent_transform_reduce ------------------------ */

template <typename T,
          typename BinaryOp_1,
          typename BinaryOp_2,
          typename L,
          typename R,
          typename... Tail>
constexpr auto adjacent_transform_reduce(T init,
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
        return adjacent_transform_reduce(std::move(reduced), transform_fn,
                                         reduce_fn, right,
                                         std::forward<Tail>(tail)...);
    }
}

// Partial Application
template <typename T>
constexpr auto adjacent_transform_reduce(T init)
{
    return detail::Partial_application_three{
        [init = std::move(init)](auto&& transform_fn, auto&& reduce_fn,
                                 auto&&... elements) {
            return adjacent_transform_reduce(
                init, std::forward<decltype(transform_fn)>(transform_fn),
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp>
constexpr auto adjacent_transform_reduce(T init, UnaryOp transform_fn)
{
    return detail::Partial_application_two{
        [init = std::move(init), transform_fn = std::move(transform_fn)](
            auto&& reduce_fn, auto&&... elements) {
            return adjacent_transform_reduce(
                init, transform_fn,
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto adjacent_transform_reduce(T init,
                                         UnaryOp transform_fn,
                                         BinaryOp reduce_fn)
{
    return detail::Partial_application_one{
        [init = std::move(init), transform_fn = std::move(transform_fn),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return adjacent_transform_reduce(
                init, transform_fn, reduce_fn,
                std::forward<decltype(elements)>(elements)...);
        }};
}

/* --------------------------- adjacent_transform --------------------------- */
template <typename BinaryOp, typename L, typename R, typename... Tail>
constexpr void adjacent_transform(BinaryOp&& transform_fn,
                                  L&& left,
                                  R&& right,
                                  Tail&&... tail)
{
    left = transform_fn(std::forward<L>(left), right);
    if constexpr (sizeof...(Tail) == 0)
        return;
    else {
        return adjacent_transform(transform_fn, right,
                                  std::forward<Tail>(tail)...);
    }
}

// Partial Application
template <typename BinaryOp>
constexpr auto adjacent_transform(BinaryOp transform_fn)
{
    return detail::Partial_application_three{
        [transform_fn = std::move(transform_fn)](auto&&... elements) {
            return adjacent_transform(
                transform_fn, std::forward<decltype(elements)>(elements)...);
        }};
}

/* -------------------------- adjacent_difference --------------------------- */
template <typename... Elements>
constexpr void adjacent_difference(Elements&&... elements)
{
    adjacent_transform([](auto const& l, auto const& r) { return r - l; },
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
    return adjacent_transform_reduce(0uL, std::equal_to<>{},
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

namespace reverse {

/* --------------------------- reverse::for_each ---------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto for_each(UnaryOp&& func, Elements&&... elements) -> void
{
    [[maybe_unused]] auto foo = 0;
    (foo = ... = (func(elements), 0));
}

template <typename UnaryOp>
constexpr auto for_each(UnaryOp func)
{
    return [func = std::move(func)](auto&&... elements) {
        return reverse::for_each(func,
                                 std::forward<decltype(elements)>(elements)...);
    };
}

/* ---------------------------- reverse::all_of ----------------------------- */
namespace detail {
template <typename UnaryOp>
constexpr auto all_of_impl(UnaryOp &&) -> bool
{
    return true;
}

// Base case of zero elements is ambiguous with partial application, new name.
template <typename UnaryOp, typename Head, typename... Tail>
constexpr auto all_of_impl(UnaryOp&& predicate, Head&& head, Tail&&... tail)
    -> bool
{
    return all_of_impl(predicate, std::forward<Tail>(tail)...) &&
           predicate(head);
}

}  // namespace detail

template <typename UnaryOp, typename... Elements>
constexpr auto all_of(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return detail::all_of_impl(std::forward<UnaryOp>(predicate),
                               std::forward<Elements>(elements)...);
}

template <typename UnaryOp>
constexpr auto all_of(UnaryOp predicate)
{
    return [f = std::move(predicate)](auto&&... elements) {
        return reverse::all_of(f,
                               std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto all(Elements&&... elements) -> bool
{
    return reverse::all_of(hal::detail::Identity{},
                           std::forward<Elements>(elements)...);
}

/* ---------------------------- reverse::any_of ----------------------------- */
namespace detail {
template <typename UnaryOp>
constexpr auto any_of_impl(UnaryOp &&) -> bool
{
    return false;
}

// Base case of zero elements is ambiguous with partial application, new name.
template <typename UnaryOp, typename Head, typename... Tail>
constexpr auto any_of_impl(UnaryOp&& predicate, Head&& head, Tail&&... tail)
    -> bool
{
    return any_of_impl(predicate, std::forward<Tail>(tail)...) ||
           predicate(head);
}

}  // namespace detail

template <typename UnaryOp, typename... Elements>
constexpr auto any_of(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return detail::any_of_impl(std::forward<UnaryOp>(predicate),
                               std::forward<Elements>(elements)...);
}

template <typename UnaryOp>
constexpr auto any_of(UnaryOp predicate)
{
    return [f = std::move(predicate)](auto&&... elements) {
        return reverse::any_of(f,
                               std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto any(Elements&&... elements) -> bool
{
    return reverse::any_of(hal::detail::Identity{},
                           std::forward<Elements>(elements)...);
}

/* --------------------------- reverse::none_of ----------------------------- */
template <typename UnaryOp, typename... Elements>
constexpr auto none_of(UnaryOp&& predicate, Elements&&... elements) -> bool
{
    return !reverse::any_of(std::forward<UnaryOp>(predicate),
                            std::forward<Elements>(elements)...);
}

template <typename UnaryOp>
constexpr auto none_of(UnaryOp predicate)
{
    return [f = std::move(predicate)](auto&&... elements) {
        return reverse::none_of(f,
                                std::forward<decltype(elements)>(elements)...);
    };
}

template <typename... Elements>
constexpr auto none(Elements&&... elements) -> bool
{
    return reverse::none_of(hal::detail::Identity{},
                            std::forward<Elements>(elements)...);
}

/* ---------------------------- reverse::reduce ----------------------------- */
namespace detail {
template <typename T, typename BinaryOp>
constexpr auto reduce_impl(T init, BinaryOp &&) -> T
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

}  // namespace detail

template <typename T, typename BinaryOp, typename... Elements>
constexpr auto reduce(T init, BinaryOp&& reduce_fn, Elements&&... elements) -> T
{
    return detail::reduce_impl(std::move(init),
                               std::forward<BinaryOp>(reduce_fn),
                               std::forward<Elements>(elements)...);
}

template <typename T>
constexpr auto reduce(T init)
{
    return hal::detail::Partial_application_two{
        [init = std::move(init)](auto&& reduce_fn, auto&&... elements) {
            return reverse::reduce(
                init, std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename BinaryOp>
constexpr auto reduce(T init, BinaryOp reduce_fn)
{
    return hal::detail::Partial_application_one{
        [init      = std::move(init),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return reverse::reduce(
                init, reduce_fn, std::forward<decltype(elements)>(elements)...);
        }};
}

/* ----------------------- reverse::partial_reduce -------------------------- */
template <typename T, typename BinaryOp, typename... Elements>
constexpr void partial_reduce(T init,
                              BinaryOp&& reduce_fn,
                              Elements&&... elements)
{
    auto wrapped_reduce = [&reduce_fn](T const& sum, auto& element) constexpr
    {
        auto const result = reduce_fn(sum, element);
        element           = result;
        return result;
    };
    reverse::reduce(init, wrapped_reduce, std::forward<Elements>(elements)...);
}

template <typename T>
constexpr auto partial_reduce(T init)
{
    return hal::detail::Partial_application_two{
        [init = std::move(init)](auto&& reduce_fn, auto&&... elements) {
            return reverse::partial_reduce(
                init, std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename BinaryOp>
constexpr auto partial_reduce(T init, BinaryOp reduce_fn)
{
    return hal::detail::Partial_application_one{
        [init      = std::move(init),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return reverse::partial_reduce(
                init, reduce_fn, std::forward<decltype(elements)>(elements)...);
        }};
}

/* -------------------------- reverse::transform ---------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto transform(UnaryOp&& transform_fn, Elements&&... elements) -> void
{
    [[maybe_unused]] auto foo = 0;
    (foo = ... =
         (elements = transform_fn(std::forward<Elements>(elements)), 0));
}

template <typename UnaryOp>
constexpr auto transform(UnaryOp transform_fn)
{
    return [f = std::move(transform_fn)](auto&&... elements) {
        return reverse::transform(
            f, std::forward<decltype(elements)>(elements)...);
    };
}

/* ---------------------- reverse::transform_reduce ------------------------- */
namespace detail {

template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto transform_reduce_impl(T init, UnaryOp&&, BinaryOp &&) -> T
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

}  // namespace detail

template <typename T, typename UnaryOp, typename BinaryOp, typename... Elements>
constexpr auto transform_reduce(T init,
                                UnaryOp&& transform_fn,
                                BinaryOp&& reduce_fn,
                                Elements&&... elements) -> T
{
    return detail::transform_reduce_impl(
        std::move(init), std::forward<UnaryOp>(transform_fn),
        std::forward<BinaryOp>(reduce_fn), std::forward<Elements>(elements)...);
}

template <typename T>
constexpr auto transform_reduce(T init)
{
    return hal::detail::Partial_application_three{
        [init = std::move(init)](auto&& transform_fn, auto&& reduce_fn,
                                 auto&&... elements) {
            return reverse::transform_reduce(
                init, std::forward<decltype(transform_fn)>(transform_fn),
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp>
constexpr auto transform_reduce(T init, UnaryOp transform_fn)
{
    return hal::detail::Partial_application_two{
        [init = std::move(init), transform_fn = std::move(transform_fn)](
            auto&& reduce_fn, auto&&... elements) {
            return reverse::transform_reduce(
                init, transform_fn,
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto transform_reduce(T init,
                                UnaryOp transform_fn,
                                BinaryOp reduce_fn)
{
    return hal::detail::Partial_application_one{
        [init = std::move(init), transform_fn = std::move(transform_fn),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return reverse::transform_reduce(
                init, transform_fn, reduce_fn,
                std::forward<decltype(elements)>(elements)...);
        }};
}

/* ------------------- reverse::partial_transform_reduce -------------------- */
namespace detail {

template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto partial_transform_reduce_impl(T init, UnaryOp&&, BinaryOp &&)
    -> T
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

}  // namespace detail

template <typename T, typename UnaryOp, typename BinaryOp, typename... Elements>
constexpr void partial_transform_reduce(T init,
                                        UnaryOp&& transform_fn,
                                        BinaryOp&& reduce_fn,
                                        Elements&&... elements)
{
    detail::partial_transform_reduce_impl(
        std::move(init), std::forward<UnaryOp>(transform_fn),
        std::forward<BinaryOp>(reduce_fn), std::forward<Elements>(elements)...);
}

template <typename T>
constexpr auto partial_transform_reduce(T init)
{
    return hal::detail::Partial_application_three{
        [init = std::move(init)](auto&& transform_fn, auto&& reduce_fn,
                                 auto&&... elements) {
            return reverse::partial_transform_reduce(
                init, std::forward<decltype(transform_fn)>(transform_fn),
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp>
constexpr auto partial_transform_reduce(T init, UnaryOp transform_fn)
{
    return hal::detail::Partial_application_two{
        [init = std::move(init), transform_fn = std::move(transform_fn)](
            auto&& reduce_fn, auto&&... elements) {
            return reverse::partial_transform_reduce(
                init, transform_fn,
                std::forward<decltype(reduce_fn)>(reduce_fn),
                std::forward<decltype(elements)>(elements)...);
        }};
}

template <typename T, typename UnaryOp, typename BinaryOp>
constexpr auto partial_transform_reduce(T init,
                                        UnaryOp transform_fn,
                                        BinaryOp reduce_fn)
{
    return hal::detail::Partial_application_one{
        [init = std::move(init), transform_fn = std::move(transform_fn),
         reduce_fn = std::move(reduce_fn)](auto&&... elements) {
            return reverse::partial_transform_reduce(
                init, transform_fn, reduce_fn,
                std::forward<decltype(elements)>(elements)...);
        }};
}

/* --------------------------- reverse::find_if ----------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto find_if(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    auto decrement_until_true =
        [still_going = true](std::size_t count, bool predicate_result) mutable {
            return still_going && !predicate_result
                       ? count - 1
                       : (still_going = false, count);
        };

    auto const result = reverse::transform_reduce(
        sizeof...(Elements) - 1, std::forward<UnaryOp>(predicate),
        decrement_until_true, std::forward<Elements>(elements)...);

    return result == -1uL ? sizeof...(Elements) : result;
}

template <typename UnaryOp>
constexpr auto find_if(UnaryOp predicate)
{
    return [predicate = std::move(predicate)](auto&&... elements) {
        return reverse::find_if(predicate,
                                std::forward<decltype(elements)>(elements)...);
    };
}

/* ------------------------- reverse::find_if_not --------------------------- */

template <typename UnaryOp, typename... Elements>
constexpr auto find_if_not(UnaryOp&& predicate, Elements&&... elements)
    -> std::size_t
{
    constexpr auto not_fn = [](auto&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&&... x) {
            return !f(std::forward<decltype(x)>(x)...);
        };
    };
    return reverse::find_if(not_fn(std::forward<UnaryOp>(predicate)),
                            std::forward<Elements>(elements)...);
}

template <typename UnaryOp>
constexpr auto find_if_not(UnaryOp predicate)
{
    return [predicate = std::move(predicate)](auto&&... elements) {
        return reverse::find_if_not(
            predicate, std::forward<decltype(elements)>(elements)...);
    };
}

/* ---------------------------- reverse::find ------------------------------- */

template <typename T, typename... Elements>
constexpr auto find(T&& x, Elements&&... elements) -> std::size_t
{
    auto equal_to_x = [&](auto y) { return y == x; };
    return reverse::find_if(equal_to_x, std::forward<Elements>(elements)...);
}

template <typename T>
constexpr auto find(T x)
{
    return [x = std::move(x)](auto&&... elements) {
        return reverse::find(x, std::forward<decltype(elements)>(elements)...);
    };
}

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

}  // namespace reverse

// to_tuple(...) from:
// reddit.com/r/cpp/comments/4yp7fv/c17_structured_bindings_convert_struct_to_a_tuple
/* ------------------------------ to_tuple ---------------------------------- */

namespace detail {

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
    else {
        return make_tup();
    }
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
        [](auto&&... x) { return std::tie(std::forward<decltype(x)>(x)...); },
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

/* -------------------------------------------------------------------------- */

// TODO?
// Partial Application?
template <typename UnaryOp, typename T>
constexpr auto struct_for_each(UnaryOp&& func, T&& aggregate) -> void
{
    return std::apply(hal::for_each(std::forward<UnaryOp>(func)),
                      hal::to_ref_tuple(std::forward<T>(aggregate)));
}

}  // namespace hal
#endif  // HAL_HPP
