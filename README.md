# Heterogeneous Algorithms Library(HAL)

Provides a basic set of algorithms that will work over a variadic template of
parameters, similar to how STL algorithms work over containers.

## The Algorithms

- Takes algorithm specific parameters first, then the parameter pack.
- Implements partial function application, all parameters not part of the pack
  can be bound by calling the function without the pack, or any partial number
  of parameters, these calls return a callable object with the given parameters
  bound.
- All algorithms are in the `halg` namespace, versions that work on elements in
  reverse order are in the `halg::reverse` namespace.
- Empty parameter packs cannot be used, will only return a callable object.

```cpp
- auto for_each(UnaryFunction, Elements...) -> void;
    Applies UnaryFunction to each Element.

- auto all_of(UnaryPredicate, Elements...) -> bool;
    Return true if all of UnaryPredicate(Elements...) are true.

- auto any_of(UnaryPredicate, Elements...) -> bool;
    Return true if any of UnaryPredicate(Elements...) are true.

- auto none_of(UnaryPredicate, Elements...) -> bool;
    Return true if none of UnaryPredicate(Elements...) are true.
```

## Examples
##### Normal Use
```cpp
auto ss = std::stringstream{};
halg::for_each([&ss](auto const& x) { ss << x; }, 2, 6.3, ' ', "hello, world!", -432, 0.5);
assert(ss.str() == "26.3 hello, world!-4320.5");
```

##### Partial Application
```cpp
auto ss          = std::stringstream{};
auto write_to_ss = halg::for_each([&ss](auto const& x) { ss << x; });

write_to_ss(2, 6.3, ' ', "hello, world!", -432);
write_to_ss(0.5);
write_to_ss();
assert(ss.str() == "26.3 hello, world!-4320.5");
```

##### Reverse Order && Partial Application
```cpp
auto ss = std::stringstream{};
auto write_to_ss_in_rev = halg::reverse::for_each([&ss](auto const& x) { ss << x; });

write_to_ss_in_rev(2, 6.3, ' ', "hello, world!", -432);
write_to_ss_in_rev(0.5);
write_to_ss_in_rev();
assert(ss.str() == "-432hello, world! 6.320.5");
```
