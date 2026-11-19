# Task 8 Analysis

`cin.fail()` detects an extraction failure such as letters entered where an integer is required. `cin.clear()` resets the stream's error state, and `ignore()` discards the invalid characters.

`iomanip` functions provide controlled money formatting.

An alternative is `getline()` followed by `stoi()` inside `try/catch`. Processing a line is O(L) for input length L and gives direct control over validation messages, while direct `cin >>` is concise for simple numeric fields.
