# Q3 Analysis

The original overload set is ambiguous because `charge(double, bool)` and `charge(double, bool=false, bool=false)` can both accept a two-argument call.

The redesign keeps one default-argument overload for kilograms and uses overloads with genuinely different first-argument types (`int` grams and `Parcel`) for different input concepts.

For `charge(2)`, the `int` grams overload is an exact match. For a `double` value, the kg overload is an exact match. A float can convert to double when the kg overload is selected explicitly.

Default arguments are supplied from the right because a caller should not have to skip an earlier parameter.

Overload resolution occurs at compile time, so there is no run-time overload-selection cost. Each selected function performs constant work, O(1), and a `Parcel` passed by const reference avoids copying the object.
