# `hal::adjacent_transform_reduce`

Transforms adjacent pairs of elements into a single value, which is then used in
a reduce operation.

```cpp
template <typename T,
          typename BinaryOp_1,
          typename BinaryOp_2,
          typename... Elements>
T adjacent_transform_reduce(T init,
                            BinaryOp_1&& transform_fn,
                            BinaryOp_2&& reduce_fn,
                            Elements&&... elements);
```

`init` is the initial value used in the reduction.

`transform_fn` takes two adjacent elements and returns a single result, which is
then used to perform the [reduce](reduce.md) operation.

`reduce_fn` should take two parameters and return a type convertible to `T`. The
first parameter is the value of the reduction so far(of type `T`), the second
parameter will be the return type of `transform_fn` for a given pair of elements
from the parameter pack. The second parameter cannot take a non-const l-value
reference.

See also [transform_reduce](transform_reduce.md).

:x: `hal::reverse::adjacent_transform_reduce(...)`

:x: Modifying Algorithm
