# Task 4 Analysis

- `a / b` is integer division because both operands are `int`; assigning the result to `double` happens afterward.
- `static_cast<double>(a) / b` converts `a` to double before division, so the result is 3.5.
- `'A' + 2` promotes the character to an integer and produces `C` when assigned to `char`.
- `ch + 1` promotes `ch` to int, so the output is the numeric character code.
- `bool flag = 5` converts non-zero integer 5 to `true`.
- Comparing signed `-1` with unsigned `1` converts the signed value to unsigned, producing a large unsigned value; therefore the condition is false.

Brace initialization such as `int x{3.7};` rejects narrowing at compile time.
