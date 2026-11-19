# Q3 Prediction

Before execution, record the overload selected for each call.

- `charge(2)` -> grams overload because `2` is an `int`.
- `charge(2.0f)` -> kg overload after conversion to `double`.
- `charge(2.0)` -> kg overload.
- `charge(2000, true)` -> grams overload.
- `charge(p)` -> `Parcel` overload.
- `charge(2.0, true, true)` -> kg overload.

The exact numeric output depends on the formulas in the implementation and should be verified by execution.
