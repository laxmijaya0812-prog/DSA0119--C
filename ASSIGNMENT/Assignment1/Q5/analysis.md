# Task 5 Analysis

- Multiplication has higher precedence than addition: `x + y * 2`.
- Parentheses force `(x + y)` before multiplication.
- `x++` uses the old value and then increments x.
- `++y` increments y before its value is used.
- The conditional operator selects the larger value in the example.
- `%` and `*` have the same precedence level, evaluated left-to-right here: `(x % 4) * 2`.
- Integer division truncates toward zero.
- For `-10 % 3`, the remainder is `-1`.
- The loop visits i = 0, 3, 6, 9, producing 18.

Avoid expressions such as `i = i++`; use a clear increment statement instead.
