# Heterogeneous Algorithm Library (HAL)

**HAL** provides algorithms operating over parameter packs and other static,
heterogeneous data structures.

```cpp
auto print = [](auto x){ std::cout << x; };
hal::for_each(print, 5, "hello", 5.2, 'a', Foo{});
```

Each algorithm can have its non-parameter pack arguments [partially
applied](docs/partial_application.md).

The top-level namespace is `hal::`. The `hal::reverse::` namespace contains
algorithms that work on elements from the last to the first element.

Namespaces `hal::memberwise::` and `hal::reverse::memberwise::` contain the same
algorithms, but operating over the members of structs, tuples, and arrays. These
take a single aggregate type argument instead of a parameter pack.

```cpp
struct Foo {
    int    a = 34;
    char   b = '#';  // 35
    double c = 7.432;
} f;

hal::memberwise::partial_sum(f);

// Partial sum is a modifying algorithm.
assert(f.a == 34);
assert(f.b == 'E');
assert(f.c == 76.432);
```

More information on structs and tuples can be found
[here](docs/tuples_structs.md).

If a function is listed as a **modifying algorithm**, it will make assignments
to the passed in parameters.

All functions are constexpr if called with constexpr parameters.

### - - > [The Algorithms](docs/toc.md) < - -

### Build

This is a single header-only library, `include/hal.hpp` includes everything
needed. If using CMake, a `hal` target is created that will add the proper
include path.

`#include <hal.hpp>`

The tests can be built with `make hal-tests` after running cmake.
