# Q6 Analysis

Operators that cannot be overloaded include `::`, `.`, `.*`, `?:`, and `sizeof`.

Important rules:
- Precedence cannot be changed.
- Arity cannot be changed.
- At least one operand must involve a user-defined type.
- Assignment `operator=` must be a non-static member.
- A friend is useful when an operator needs symmetric access or when the left operand is a type such as `std::ostream`.

The class follows the Rule of Three because it owns a dynamically allocated character array.

For `a + b + c`, intermediate concatenations copy characters. With lengths n1, n2 and n3, a left-associative evaluation copies approximately `(n1+n2) + (n1+n2+n3)` characters, ignoring constant factors.
