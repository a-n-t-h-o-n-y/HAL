#ifndef HALG_HPP
#define HALG_HPP
#include <tuple>
#include <type_traits>
#include <utility>

namespace halg {
namespace detail {

/// Creates a curried function that can bind a variable number of arguments.
template <typename F, typename... CapturedArgs>
class Curried {
   private:
    F func_;
    std::tuple<CapturedArgs...> captured_;

   public:
    Curried(F function, CapturedArgs... args)
        : func_{function}, captured_{capture_by_copy(std::move(args)...)}
    {}

    Curried(F function, std::tuple<CapturedArgs...> args)
        : func_{function}, captured_{std::move(args)}
    {}

   public:
    template <typename... NewArgs>
    auto operator()(NewArgs&&... args) const
    {
        auto new_args = capture_by_copy(std::forward<NewArgs>(args)...);
        auto all_args = std::tuple_cat(captured_, std::move(new_args));
        if constexpr (std::is_invocable_v<F, CapturedArgs..., NewArgs...>)
            return std::apply(func_, std::move(all_args));
        else
            return Curried<F, CapturedArgs..., NewArgs...>{func_,
                                                           std::move(all_args)};
    }

   private:
    template <typename... Args>
    static auto capture_by_copy(Args&&... args)
    {
        return std::tuple<std::decay_t<Args>...>{std::forward<Args>(args)...};
    }
};

}  // namespace detail

}  // namespace halg
#endif  // HALG_HPP
