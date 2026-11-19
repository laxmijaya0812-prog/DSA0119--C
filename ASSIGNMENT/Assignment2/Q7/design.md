# Q7 – Diagnose, Redesign and Validate

## Selected limitation

The Q4 fixed-array transaction history uses shifting when the 11th transaction arrives. With capacity `k`, insertion after the buffer is full takes O(k).

## A. Diagnose

The limitation is caused by physically moving all stored transactions one position to the left before inserting the new one.

## B. Design objective

Replace shifting with a circular-buffer design so that adding a transaction remains O(1), while retaining the same public deposit/withdraw/statement behaviour.

## C. Revised solution

Maintain:
- `transactions[CAPACITY]`
- `start` – index of the oldest item
- `count` – number of stored items

When the buffer is full, advance `start` modulo `CAPACITY` and write the new transaction at the next circular position.

### Pseudocode

```text
addTransaction(x):
    if count < CAPACITY:
        index = (start + count) mod CAPACITY
        transactions[index] = x
        count++
    else:
        start = (start + 1) mod CAPACITY
        index = (start + count - 1) mod CAPACITY
        transactions[index] = x
```

## D. Prediction

For a full history, the circular design should eliminate O(k) shifting and make insertion O(1). For only 10 items, measured wall-clock differences may be small.

## E. Validate

Run both original and redesigned implementations using the same number of transactions. Record insertion timings and confirm that the newest 10 transactions are identical.

## F. Defence

The circular buffer is preferable when transaction history grows or updates are frequent. The fixed shifting array is simpler for very small capacities and may be easier to understand in introductory code. The redesign uses the same O(k) space but improves insertion from O(k) to O(1).
