# Task 2 Analysis

Tokens include keywords (`constexpr`, `enum`, `if`), identifiers (`MIDDLE_PAISE`, `hour`), constants (`20`, `5000`), operators (`>=`, `&&`, `==`) and punctuators (`;`, `{`, `}`).

`constexpr` is preferred for compile-time fare constants because it is typed, scoped and checked by the compiler. `#define` is textual substitution and has weaker type/scope behaviour.

`enum class` is safer than an unscoped enum because its enumerators are scoped and do not implicitly convert to int in ordinary expressions.

Identifiers should use meaningful names and consistent conventions such as `snake_case` for variables/functions or `PascalCase` for classes.
