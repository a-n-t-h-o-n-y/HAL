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

/* -------------------------------- find_if --------------------------------- */
template <typename UnaryOp, typename... Pack>
constexpr auto find_if(UnaryOp&& predicate, Pack&&... elements) -> std::size_t
{
    auto increment_until_true = [still_going = true](std::size_t count,
                                                     auto boolean) mutable {
        return still_going && !boolean ? count + 1
                                       : (still_going = false, count);
    };
    return transform_reduce(0uL, std::forward<UnaryOp>(predicate),
                            increment_until_true,
                            std::forward<Pack>(elements)...);
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

template <typename UnaryOp, typename... Pack>
constexpr auto find_if_not(UnaryOp&& predicate, Pack&&... elements)
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
                   std::forward<Pack>(elements)...);
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

template <typename T, typename... Pack>
constexpr auto find(T&& x, Pack&&... elements) -> std::size_t
{
    auto equal_to_x = [&](auto y) { return y == x; };
    return find_if(equal_to_x, std::forward<Pack>(elements)...);
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

/* ---------------------------------- get ----------------------------------- */

template <std::size_t I, typename... Elements>
constexpr auto get(Elements&&... elements)
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
constexpr auto first(Elements&&... elements)
{
    return get<0>(std::forward<Elements>(elements)...);
}

/* --------------------------------- last ----------------------------------- */

template <typename... Elements>
constexpr auto last(Elements&&... elements)
{
    return get<sizeof...(Elements) - 1>(std::forward<Elements>(elements)...);
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

/* --------------------------- reverse::find_if ----------------------------- */

template <typename UnaryOp, typename... Pack>
constexpr auto find_if(UnaryOp&& predicate, Pack&&... elements) -> std::size_t
{
    auto decrement_until_true =
        [still_going = true](std::size_t count, bool predicate_result) mutable {
            return still_going && !predicate_result
                       ? count - 1
                       : (still_going = false, count);
        };

    auto const result = reverse::transform_reduce(
        sizeof...(Pack) - 1, std::forward<UnaryOp>(predicate),
        decrement_until_true, std::forward<Pack>(elements)...);

    return result == -1uL ? sizeof...(Pack) : result;
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

template <typename UnaryOp, typename... Pack>
constexpr auto find_if_not(UnaryOp&& predicate, Pack&&... elements)
    -> std::size_t
{
    constexpr auto not_fn = [](auto&& f) {
        return [f = std::forward<decltype(f)>(f)](auto&&... x) {
            return !f(std::forward<decltype(x)>(x)...);
        };
    };
    return reverse::find_if(not_fn(std::forward<UnaryOp>(predicate)),
                            std::forward<Pack>(elements)...);
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

template <typename T, typename... Pack>
constexpr auto find(T&& x, Pack&&... elements) -> std::size_t
{
    auto equal_to_x = [&](auto y) { return y == x; };
    return reverse::find_if(equal_to_x, std::forward<Pack>(elements)...);
}

template <typename T>
constexpr auto find(T x)
{
    return [x = std::move(x)](auto&&... elements) {
        return reverse::find(x, std::forward<decltype(elements)>(elements)...);
    };
}

/* -------------------------------------------------------------------------- */

}  // namespace reverse
}  // namespace hal
#endif  // HAL_HPP
