# `hal::adjacent_difference`

Calculates the difference between each pair of consecutive elements, stores the
result in the first element of the pair.

```cpp
template <typename... Elements>
void adjacent_difference(Elements&&... elements);
```

The difference is found by subtracting the left element from the right element
for each pair. The return type for `operator-(...)` has to be assignable to the
first element of the pair. The last element in the pack remains unmodified.

:x: `hal::reverse::adjacent_difference(...)`

:heavy_check_mark: Modifying Algorithm
