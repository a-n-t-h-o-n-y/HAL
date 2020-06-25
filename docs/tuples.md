# Tuples

```cpp
auto const append_to = [](std::string& s) {
    return [&s](auto x) {
        auto ss = std::stringstream{};
        ss << x;
        s.append(ss.str());
    };
};

auto s           = std::string{};
auto append_each = hal::for_each(append_to(s));
std::apply(append_each, foo);
CHECK(s == "43.2y");
```
