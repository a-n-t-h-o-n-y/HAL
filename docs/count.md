# `hal::count`

Count the number of elements equivalent to parameter `x`.

```cpp
template <typename T, typename... Elements>
std::size_t count(T&& x, Elements&&... elements);
```

Uses `operator==` to perform the comparison, returns the count as an unsigned
value.

:x: `hal::reverse::count(...)`

:x: Modifying Algorithm

# `hal::count_if`

Count the number of elements where `predicate(element)` returns true.

```cpp
template <typename UnaryOp, typename... Elements>
std::size_t count_if(UnaryOp&& predicate, Elements&&... elements);
```

`predicate` is any callable accepting a single element and returning a type
convertible to bool. Returns the count as an unsigned value.

:x: `hal::reverse::count(...)`

:x: Modifying Algorithm

[Examples](../tests/count.test.cpp)
