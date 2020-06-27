# `hal::all_of`

Check if all of the elements satisfy some predicate.

```cpp
template <typename UnaryOp, typename... Elements>
bool all_of(UnaryOp&& predicate, Elements&&... elements);
```

`predicate` is any callable accepting a single element and returning a type
convertible to bool.

### `hal::all`

Convinience function where the predicate of `all_of(...)` is the identity
function.

```cpp
template <typename... Elements>
bool all(Elements&&... elements);
```

:heavy_check_mark: `hal::reverse::all_of(...)`

:heavy_check_mark: `hal::reverse::all(...)`

:x: Modifying Algorithm

# `hal::any_of`

Check if at least one element satisfies some predicate.

```cpp
template <typename UnaryOp, typename... Elements>
bool any_of(UnaryOp&& predicate, Elements&&... elements);
```

`predicate` is any callable accepting a single element and returning a type
convertible to bool.

### `hal::any`

Convinience function where the predicate of `any_of(...)` is the identity
function.

```cpp
template <typename... Elements>
bool any(Elements&&... elements);
```

:heavy_check_mark: `hal::reverse::any_of(...)`

:heavy_check_mark: `hal::reverse::any(...)`

:x: Modifying Algorithm

# `hal::none_of`

Check if none of the elements satisfy some predicate.

```cpp
template <typename UnaryOp, typename... Elements>
bool none_of(UnaryOp&& predicate, Elements&&... elements);
```

`predicate` is any callable accepting a single element and returning a type
convertible to bool.

### `hal::none`

Convinience function where the predicate of `none_of(...)` is the identity
function.

```cpp
template <typename... Elements>
bool none(Elements&&... elements);
```

:heavy_check_mark: `hal::reverse::none_of(...)`

:heavy_check_mark: `hal::reverse::none(...)`

:x: Modifying Algorithm
