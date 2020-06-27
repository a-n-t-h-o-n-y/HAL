# `hal::adjacent_transform`

Modifies each element of a parameter pack by applying a function to adjacent
pairs of elements and assigning the result to the first element of the pair.

```cpp
template <typename BinaryOp, typename... Elements>
void adjacent_transform(BinaryOp&& transform_fn, Elements&&... elements);
```

`transform_fn` will be called with each adjacent pair of elements. It should
accept two const l-value references to element types and return a type that can
be assigned to the first parameter type.

:x: `hal::reverse::adjacent_transform(...)`

:heavy_check_mark: Modifying Algorithm
