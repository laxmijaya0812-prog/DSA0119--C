# Q2 Analysis

`highest()` returns a reference directly to an existing array element. Therefore the expression `highest(marks, 4) += 2` modifies the original element.

Returning a reference to a local variable is invalid because the local object is destroyed when the function returns. Using that dangling reference is undefined behaviour. A compiler such as g++ may warn about returning a reference to a local variable.

### Reference vs const reference vs copy

- `int&`: caller can modify the referred object.
- `const int&`: caller can read without copying but cannot modify through the reference.
- `int`: caller receives an independent copy.

If the array becomes a growing `std::vector`, references may become invalid after operations that reallocate the vector.

Finding the maximum is O(n) and uses O(1) extra space. Returning an index is often safer when later vector growth is possible; a pointer has similar invalidation concerns to a reference.
