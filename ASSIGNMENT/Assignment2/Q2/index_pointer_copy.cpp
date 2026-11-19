#include <iostream>

int highestIndex(const int marks[], int n) {
    int index = 0;
    for (int i = 1; i < n; ++i)
        if (marks[i] > marks[index]) index = i;
    return index;
}

int main() {
    int marks[] = {72, 91, 84, 88};
    int index = highestIndex(marks, 4);

    std::cout << "Index: " << index << '\n';
    marks[index] += 2;
    std::cout << "Updated mark: " << marks[index] << '\n';
}
