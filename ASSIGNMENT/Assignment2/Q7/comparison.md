# Q7 Comparison

| Property | Original shifting | Circular buffer |
|---|---|---|
| Storage | O(k) | O(k) |
| Add before full | O(1) | O(1) |
| Add when full | O(k) | O(1) |
| Public concept | transaction history | transaction history |
| Main trade-off | simpler logic | index management |

### Evidence to collect

1. Compile both programs with the same compiler.
2. Run with the same transaction sequence.
3. Capture output screenshots.
4. For a meaningful benchmark, use a larger repeated workload rather than only 10 transactions.
5. Add the actual measurements to `comparison_results.md`.
