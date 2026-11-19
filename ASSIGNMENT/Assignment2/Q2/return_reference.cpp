#include <iostream>
#include <algorithm>

int& highest(int marks[], int n) {
    int index = 0;
    for (int i = 1; i < n; ++i) {
        if (marks[i] > marks[index]) index = i;
    }
    return marks[index];
}

// Intentionally unsafe example for compiler-warning evidence.
// Do NOT dereference the returned reference.
int& badReference() {
    int local = 42;
    return local;
}

int main() {
    int marks[] = {72, 91, 84, 88};

    std::cout << "Highest before: " << highest(marks, 4) << '\n';
    highest(marks, 4) += 2;
    std::cout << "Highest after : " << highest(marks, 4) << '\n';

    int* highestPtr = &highest(marks, 4);
    std::cout << "Pointer value  : " << *highestPtr << '\n';

    return 0;
}
