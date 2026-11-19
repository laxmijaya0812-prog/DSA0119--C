# Task 6 – Standard Pseudocode

```text
BEGIN
    SET balance to 10000 paise

    FOR each trip
        SELECT zone
            Inner   -> base fare = 1000 paise
            Middle  -> base fare = 2000 paise
            Outer   -> base fare = 3000 paise
        END SELECT

        IF peak is true
            surcharge = base fare * 20 / 100
            fare = base fare + surcharge
        ELSE
            fare = base fare
        END IF

        IF balance - fare >= 5000
            balance = balance - fare
            PRINT fare and balance
        ELSE
            PRINT denied
        END IF
    END FOR
END
```

The boundary rule must be checked at exactly 10:00 AM and 8:00 PM. The stated peak interval is 08:00–10:00 and 17:00–20:00 using the assignment's boundary convention; the implementation uses `hour >= 8 && hour < 10` and `hour >= 17 && hour < 20`.
