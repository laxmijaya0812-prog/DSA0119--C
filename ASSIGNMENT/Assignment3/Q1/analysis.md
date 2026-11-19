# Q1 Analysis

- A compiler-generated default constructor is available when the class has no user-declared constructor. Once a constructor is declared, the compiler does not automatically provide the same default constructor.
- Member initializer lists directly initialise data members and are required for const/reference members. They also avoid default construction followed by assignment.
- Members are initialised in declaration order, not the order written in the initializer list.
- `Book` validates `copies` through a common constructor path, so the validation rule is centralised.
- Each object construction is O(1) for the fixed set of members and uses O(1) additional space.
- An alternative single constructor with default arguments can reduce the number of declarations, but overloaded constructors can express the three intended creation cases more clearly.
- `explicit` is useful for constructors that can perform implicit one-argument conversions. For example, `Book b = "C++ Basics";` is prevented when the title constructor is explicit.
