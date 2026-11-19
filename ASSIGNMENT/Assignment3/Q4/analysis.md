# Q4 Analysis

- `+` returns a new `Money` object because it represents a value-producing operation.
- `+=` modifies the current object and returns `*this` by reference so chained updates avoid an unnecessary copy.
- `<<` is a non-member friend because the left operand is `std::ostream`.
- `+` can be viewed conceptually as `a.operator+(b)` for the member implementation.
- All implemented arithmetic/comparison operations are O(1) time and O(1) extra space.
- Money is stored in paise using an integer representation, avoiding floating-point rounding issues for ordinary rupee/paise arithmetic.
