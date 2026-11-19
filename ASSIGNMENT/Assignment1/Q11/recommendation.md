# Task 11 – Final Recommendation

Recommend:
- `enum class Zone` for type-safe zones.
- `constexpr` fare constants.
- `long long` paise for money.
- private balance in `SmartCard`.
- a central `FareTable`/fare-calculation function.
- explicit peak-hour boundary checks.
- validation before deducting a fare.
- automated test cases for normal and boundary journeys.

A fare revision should change the central fare table/constants and then rerun the complete test suite.

Limitations:
- The current model represents a simplified zone-based fare.
- Production deployment would require authenticated cards, secure transactions and a configurable fare source.

The design scales in time O(1) per journey, while cumulative processing over J journeys is O(J).
