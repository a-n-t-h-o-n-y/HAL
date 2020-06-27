# `hal::for_each`

Apply a function to each element in a parameter pack.

```cpp
template <typename UnaryOp, typename... Elements>
void for_each(UnaryOp&& f, Elements&&... elements);
```

Parameter `f` will be called with each element, one at a time. Its signature
should accept a single, const l-value element, any return value is ignored. If
`f` is a function object with an overloaded call operator for each type in the
pack, the correct overload will be applied for each element.

The signature of this function does not stop you from modifying the elements
with `f`, taking a non-const reference.

:heavy_check_mark: `hal::reverse::for_each(...)`

:x: Modifying Algorithm
