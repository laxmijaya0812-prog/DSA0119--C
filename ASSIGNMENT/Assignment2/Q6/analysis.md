# Q6 Analysis

A friend function is appropriate when an operation conceptually treats two `Time` objects equally. It is called as an ordinary function, not through an object, but it can access the private `h` and `m` members because friendship is explicitly granted.

A member-function alternative could be `a.minutesTo(b)` and `a.isEarlier(b)`. Public getters would preserve private data but expose more interface surface.

`inline` permits the compiler to consider inline substitution; it does not force the compiler to inline. At `-O2`, the compiler may optimize the tiny function regardless of the keyword.

Each comparison is O(1) time and O(1) extra space. Run the timing program at `-O0` and `-O2` and record actual measurements in `timing_results.md`.
