# Q4 Analysis

The transaction array is private so outside code cannot arbitrarily change account history or break the class invariant.

With a fixed array and shifting, inserting transaction 11 costs O(k), where k is the number of stored transactions. With a circular buffer, insertion can be O(1).

For 50 transactions, the class mainly changes its capacity constant and array storage; the public deposit/withdraw interface can remain unchanged. A `std::deque` also avoids manual shifting and provides convenient push-back/pop-front operations.
