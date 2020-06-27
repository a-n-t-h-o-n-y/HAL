# `hal::reduce`

Performs a reduce(fold/accumulate) over a parameter pack.

```cpp
template <typename T, typename BinaryOp, typename... Elements>
T reduce(T init, BinaryOp&& reduce_fn, Elements&&... elements);
```

`init` is the initial value used in the reduction.

`reduce_fn` should take two parameters and return a type convertible to `T`. The
first parameter is the value of the reduction so far(of type `T`), the second
parameter will be an element from the passed in parameter pack. Using a function
object with the call operator overloaded can give varying behavior based on the
type of element passed in.

:heavy_check_mark: `hal::reverse::reduce(...)`

:x: Modifying Algorithm

[Examples](../tests/reduce.test.cpp)
