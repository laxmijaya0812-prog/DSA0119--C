# Q3 Analysis

A row-pointer representation allocates one pointer array plus one array for each row, giving `rows + 1` allocations when rows are non-zero.

- Seat access: O(1)
- Booking/cancelling: O(1)
- Construction: O(rows * seats)
- Destruction: O(rows * seats)
- Seat storage: O(rows * seats)
- Row-pointer overhead: O(rows)

Forgetting `delete` causes a memory leak. Using `delete` instead of `delete[]` for an array is undefined behavior.

A destructor has no return type or parameters and cannot be overloaded because an object has one destruction operation selected automatically by its lifetime.
