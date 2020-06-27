# HAL Overview

non-mutating means the function does not assign over any of the elements in the
pack itself.

mutating means the function explicitly writes over values of the parameter pack,
on your behalf.

All functions are constexpr if called with constexpr parameters.

All functions in the `hal` namespace iterate forward, from the first element to
the last. All functions in the `hal::reverse` namespace iterate backwards, from
the last element to the first.

example of static visit pattern over a struct, or array
