# Task 12 – Day-4 Design Challenge

## Requirement impact

Affected elements:
- `Zone` gains `Airport`.
- fare table gains Rs 60.
- discount logic gains a weekend rule.
- peak and weekend rules must be mutually exclusive.
- `MonthlyPass` is added for two chosen zones.
- `SmartCard` remains useful for balance protection.

## Decision: MODIFY

The original SmartCard abstraction can be retained. The fare calculation should be modified to use a central fare table and separate discount policy logic.

## Worked examples

- Airport off-peak = Rs 60.
- Airport peak = Rs 72 (20% surcharge).
- Airport weekend at a peak hour = Rs 54 because weekend discount does not combine with peak surcharge.
- A monthly pass covers exactly its two selected zones.

## Trade-off

A table-driven design becomes easier to extend when another zone is added. A separate policy object would become preferable if many discount types are introduced.
