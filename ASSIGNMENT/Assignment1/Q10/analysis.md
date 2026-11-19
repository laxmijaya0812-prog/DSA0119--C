# Task 10 Analysis

Run the program and record the actual values from your compiler.

The integer-paise total is exact for the tested integer fare. A double total may display a tiny representation difference because decimal fractions such as 0.10 are not generally represented exactly in binary floating point.

`int` can overflow when a cumulative total exceeds approximately 2.147 billion for a signed 32-bit int. `long long` provides a much larger range and is preferred for network totals.
