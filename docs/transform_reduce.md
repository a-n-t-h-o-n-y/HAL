# `hal::transform_reduce`

Applies a transformation on each element and reduces the result of that
transformation for each element in a parameter pack.

```cpp
template <typename T, typename UnaryOp, typename BinaryOp, typename... Elements>
T transform_reduce(T init,
                   UnaryOp&& transform_fn,
                   BinaryOp&& reduce_fn,
                   Elements&&... elements);
```

`init` is the initial value used in the reduction.

`transform_fn` will be called with each element, one at a time. Its signature
should accept a single, const l-value element. The return type needs to be
assignable to type `T`.

`reduce_fn` should take two parameters and return a type convertible to `T`. The
first parameter is the value of the reduction so far(of type `T`), the second
parameter will be the return type of `transform_fn` for a given element from the
passed in parameter pack. The second parameter cannot take a non-const l-value
reference.

:heavy_check_mark: `hal::reverse::transform_reduce(...)`

:x: Modifying Algorithm
