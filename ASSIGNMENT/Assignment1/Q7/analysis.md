# Task 7 Analysis

The procedural version keeps balance in a variable and uses free functions.

The OOP version encapsulates balance inside `SmartCard`. Outside code cannot directly assign an invalid negative balance because the data member is private.

Both designs have O(1) time per journey and O(1) memory per card.

The OOP design is easier to change because fare and balance rules can be modified in the class without exposing internal state to the rest of the program.
