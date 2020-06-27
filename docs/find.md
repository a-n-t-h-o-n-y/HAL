# `hal::find_if`

Return the index of the first element that matches some predicate.

```cpp
template <typename UnaryOp, typename... Elements>
std::size_t find_if(UnaryOp&& predicate, Elements&&... elements);
```

`predicate` is any callable accepting a single element and returning a type
convertible to bool. The return value index is zero based, will return
`sizeof...(Elements...)` if no match is found.

:heavy_check_mark: `hal::reverse::find_if(...)`

:x: Modifying Algorithm

# `hal::find_if_not`

Return the index of the first element that does not match some predicate.

```cpp
template <typename UnaryOp, typename... Elements>
std::size_t find_if_not(UnaryOp&& predicate, Elements&&... elements)
```

`predicate` is any callable accepting a single element and returning a type
convertible to bool. `predicate` is used as if it is wrapped in `std::not_fn` to
find a `true` result.  The return value index is zero based, will return
`sizeof...(Elements...)` if no match is found.

:heavy_check_mark: `hal::reverse::find_if_not(...)`

:x: Modifying Algorithm

# `hal::find`

Return the index of the first element that is equal to a given object.

```cpp
template <typename T, typename... Elements>
std::size_t find(T&& x, Elements&&... elements);
```

Uses `operator==` to perform the equality check for each element against
parameter `x`, until a match is found.  The return value index is zero based,
will return `sizeof...(Elements...)` if no match is found.

:heavy_check_mark: `hal::reverse::find(...)`

:x: Modifying Algorithm
