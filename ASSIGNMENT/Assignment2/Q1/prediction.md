# Q1 Prediction and Explanation

## Prediction for the supplied program

Before execution:

```text
11 12 13
100 13
16 8 6
```

## Reason

- `a` is passed by value, so changing parameter `a` does not change the caller's `a`.
- `b` is passed by reference, so the caller's `b` changes from 2 to 12.
- `c` is passed by pointer and dereferenced, so the caller's `c` changes from 3 to 13.
- `bigger(a,c) = 100` returns a reference to the larger variable. After `change`, `c` is 100, so the second line becomes `1 100`.
- `show(1)` uses `y=5,z=10`, giving 16.
- `show(1,2)` uses `z=10`, giving 13.
- `show(1,2,3)` gives 6.

Correct final prediction:

```text
1 12 13
1 100
16 13 6
```
