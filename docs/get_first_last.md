# `hal::get`

Retrieve an element, via an index, from a parameter pack.

```cpp
template <std::size_t I, typename... Elements>
decltype(auto) get(Elements&&... elements);
```

Returns a reference(l or r value) to the element at index `I`. Will fail to
compile if `I` is out of range of the parameter pack.

:x: `hal::reverse::get(...)`

:x: Modifying Algorithm

# `hal::first`

Retrieve the first element from a parameter pack.

```cpp
template <typename... Elements>
decltype(auto) first(Elements&&... elements);
```

Returns a reference(l or r value) to the first element. Will fail to compile if
`elements...` is empty.

:x: `hal::reverse::first(...)`

:x: Modifying Algorithm

# `hal::last`

Retrieve the last element from a parameter pack.

```cpp
template <typename... Elements>
decltype(auto) last(Elements&&... elements);
```

Returns a reference(l or r value) to the last element. Will fail to compile if
`elements...` is empty.

:x: `hal::reverse::last(...)`

:x: Modifying Algorithm
