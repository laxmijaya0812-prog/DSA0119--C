# Q1 Analysis

## Storage and caller modification

| Method | Parameter | Effect on caller |
|---|---|---|
| Value | local copy | No |
| Reference | alias to caller | Yes |
| Pointer | address copied, then dereferenced | Yes |

A `const` reference should be used when a function must read an object without copying it and must not modify the caller's object.

For multiple concessions, changing the fee in place is suitable when the caller owns the fee and wants sequential updates. Returning the new fee is preferable when a non-mutating, expression-friendly interface is required.

All arithmetic here is O(1). For a `long long`, by-value copying uses O(1) additional parameter storage. A large object passed by value copies the whole object, while reference and const-reference pass an alias/address-sized parameter.
