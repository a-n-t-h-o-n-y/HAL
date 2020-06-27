# `hal::transform`

Apply a function to each element in a parameter pack, and assign the result to
that element.

```cpp
template <typename UnaryOp, typename... Elements>
void transform(UnaryOp&& transform_fn, Elements&&... elements);
```

`transform_fn` will be called with each element, one at a time. Its signature
should accept a single, const l-value element. The return type needs to be
assignable to the element type that it was called on.

If `transform_fn` is a function object with an overloaded call operator for each
type in the pack, the correct overload will be applied for each element.

:heavy_check_mark: `hal::reverse::transform(...)`

:heavy_check_mark: Modifying Algorithm
