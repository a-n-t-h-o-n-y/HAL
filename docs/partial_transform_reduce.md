# `hal::partial_transform_reduce`

Performs a [tranform_reduce](transform_reduce.md) over a parameter pack,
assigning the partially reduced result to each element it encounters.

```cpp
template <typename T, typename UnaryOp, typename BinaryOp, typename... Elements>
void partial_transform_reduce(T init,
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

Each type in the parameter pack needs to be assignable from a `T`.

:heavy_check_mark: `hal::reverse::partial_transform_reduce(...)`

:heavy_check_mark: Modifying Algorithm

[Examples](../tests/partial_transform_reduce.test.cpp)
