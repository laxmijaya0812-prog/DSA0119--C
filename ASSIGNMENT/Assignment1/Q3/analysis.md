# Task 3 Analysis

Recommended types:
- `bool` for concession/peak flags.
- `char` for a compact zone code.
- `int` for hour and percentages.
- `long long` for money in paise and large totals.
- `unsigned int` can represent a non-negative journey count when appropriate.
- `double` only where fractional scientific/measurement values are required.
- `auto` when the initializer clearly communicates the intended type.

Money should be stored as integer paise because binary floating point cannot exactly represent every decimal fraction.

A daily network total for millions of fares can exceed the range of a small integer type, so `long long` is safer for cumulative money.
