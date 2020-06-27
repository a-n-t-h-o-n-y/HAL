# `hal::partial_reduce`

Performs a [reduce](reduce.md) over a parameter pack, writing over each
element with the current value of the reduce.

```cpp
template <typename T, typename BinaryOp, typename... Elements>
void partial_reduce(T init, BinaryOp&& reduce_fn, Elements&&... elements);
```

`init` is the initial value used in the reduction.

`reduce_fn` should take two parameters and return a type convertible to `T`. The
first parameter is the value of the reduction so far(of type `T`), the second
parameter will be an element from the passed in parameter pack.

Each type in the parameter pack needs to be assignable from a `T`.

:heavy_check_mark: `hal::reverse::partial_reduce(...)`

:heavy_check_mark: Modifying Algorithm

# `hal::partial_sum`

Convinience function where `reduce_fn` of `partial_reduce(...)` is
`std::plus<>`, `T` is the type of the first element, and `init` is `0`.

```cpp
template <typename... Elements>
void partial_sum(Elements&&... elements);
```

:heavy_check_mark: `hal::reverse::partial_sum(...)`

:heavy_check_mark: Modifying Algorithm

# `hal::partial_difference`

Convinience function where `reduce_fn` of `partial_reduce(...)` is
`std::minus<>`, `T` is the type of the first element, and `init` is `0`.

```cpp
template <typename... Elements>
void partial_difference(Elements&&... elements);
```

:heavy_check_mark: `hal::reverse::partial_difference(...)`

:heavy_check_mark: Modifying Algorithm

# `hal::partial_product`

Convinience function where `reduce_fn` of `partial_reduce(...)` is
`std::multiplies<>`, `T` is the type of the first element, and `init` is `1`.

```cpp
template <typename... Elements>
void partial_product(Elements&&... elements);
```

:heavy_check_mark: `hal::reverse::partial_product(...)`

:heavy_check_mark: Modifying Algorithm

# `hal::partial_quotient`

Convinience function where `reduce_fn` of `partial_reduce(...)` is
`std::divides<>`, `T` is the type of the first element, and `init` is `1`.

```cpp
template <typename... Elements>
void partial_quotient(Elements&&... elements)
```

:heavy_check_mark: `hal::reverse::partial_quotient(...)`

:heavy_check_mark: Modifying Algorithm

[Examples](../tests/partial_reduce.test.cpp)
