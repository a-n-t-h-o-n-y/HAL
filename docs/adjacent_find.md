# `hal::adjacent_find`

Searches for two consecutive elements, giving the index of the first element of
the pair.

```cpp
template <typename... Elements>
std::size_t adjacent_find(Elements&&... elements);
```

Returns sizeof...(elements) - 1 if no pairs found. Cannot be called with fewer
than two elements. Uses `std::equal_to<>` to perform the comparison between
adjacent elements.

:x: `hal::reverse::adjacent_find(...)`

:x: Modifying Algorithm
