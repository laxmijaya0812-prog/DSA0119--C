# Q2 – Copy Constructors: Shallow and Deep Copies

The final program implements the Rule of Three:
1. Copy constructor
2. Copy assignment operator
3. Destructor

The copy constructor allocates a new array, so modifying `q` does not modify `p`.

Expected final data line:
```text
Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS
```

Sanitizer:
```powershell
g++ -std=c++17 -Wall -Wextra -pedantic -fsanitize=address prescription.cpp -o prescription_asan.exe
.\prescription_asan.exe
```

For a shallow version, remove the custom copy constructor and run it under AddressSanitizer. The resulting ownership error is expected because two objects would point to the same array and both destructors would attempt to release it.

Complexity:
- Deep copy time: O(n)
- Deep copy additional space: O(n)
- Shallow pointer copy: O(1), but unsafe ownership
