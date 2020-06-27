# Partial Application

Each algorithm can have parameters partially applied to it, parameter packs
cannot be partially applied.

```cpp
auto print = [](auto const& x){ std::cout << x; };

// void hal::for_each(function, elements...)
auto print_each = hal::for_each(print);

print_each(1, 'a', "hello", ' ', "world");
```

Calling an algorithm with fewer parameters than required will return a new
function object that has those parameters stored within it.

Partial application can only happen once, the next call on the returned object
needs to complete the call by providing the rest of the arguments.

```cpp
// T hal::transform_reduce(init, transform_fn, reduce_fn, elements...)
auto ten_reduce = hal::transform_reduce(10);

auto error = ten_reduce(std::identity{}); // Error: Need to make a full call.

auto sum = ten_reduce(std::identity{}, std::plus<>{}, 0, 1, 2, 'a', 4.99);
```
