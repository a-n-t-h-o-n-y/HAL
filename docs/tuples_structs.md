# Tuples

Tuples can be used as if they are parameter packs with the `std::apply`
function.

Any algorithm used with `std::apply` will need to have its non-parameter pack
parameters [partially applied](partial_application.md), so that the parameter
pack elements can be applied by themselves from the tuple.

```cpp
auto print = [](auto const& x){ std::cout << x; };

// void hal::for_each(function, elements...)
auto print_each = hal::for_each(print);

std::apply(print_each, std::tuple{1, "hello, world!", Foo{}});
```

# Structs

The algorithms can also be used across `struct` members by converting a struct
to a tuple, and then using `std::apply`. This conversion can be performed with
the `hal::to_tuple` utility function.

```cpp
template <typename T>
auto to_tuple(T&& object);
```

```cpp
auto print = [](auto const& x){ std::cout << x; };
auto print_each = hal::for_each(print);

struct Foo {
    int    a = 34;
    char   b = '#';
    double c = 7.432;
} f;

std::apply(print_each, hal::to_tuple(f)});
```

Since this conversion copies/moves items from the given struct, using a
modifying alorithm on this tuple will not modify the elements within the struct,
only in the tuple.

There is `hal::to_ref_tuple` which will create a tuple of references to the
members of the struct, via the `std::tie` function, and performing modifying
algorithms across these tuples will result in the values of the struct changing.

```cpp
template <typename T>
auto to_ref_tuple(T&& object);
```

```cpp
struct Foo {
    int    a = 34;
    char   b = '#';  // 35
    double c = 7.432;
} f;

auto ps = [](auto&&... x){ hal::partial_sum(std::forward<decltype(x)>(x)...); };
std::apply(ps, hal::to_ref_tuple(f));

assert(f.a == 34);
assert(f.b == 'E');
assert(f.c == 76.432);
```

This library also provides `hal::from_tuple`, which will brace initialize an
object of type T from the elements in the tuple. The standard library provides
`std::make_from_tuple` which will perform a parentheses initialization.

```cpp
template <typename T, typename Tuple>
auto from_tuple(Tuple&& t) -> T;
```

[Examples](../tests/tuples.test.cpp)
