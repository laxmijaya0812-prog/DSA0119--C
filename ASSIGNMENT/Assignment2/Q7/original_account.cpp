#include <iostream>

class ShiftAccount {
    static constexpr int CAPACITY = 10;
    double transactions[CAPACITY]{};
    int count = 0;

public:
    void add(double amount) {
        if (count < CAPACITY) {
            transactions[count++] = amount;
        } else {
            for (int i = 1; i < CAPACITY; ++i)
                transactions[i - 1] = transactions[i];
            transactions[CAPACITY - 1] = amount;
        }
    }

    void print() const {
        for (int i = 0; i < count; ++i)
            std::cout << transactions[i] << ' ';
        std::cout << '\n';
    }
};

int main() {
    ShiftAccount account;
    for (int i = 1; i <= 12; ++i)
        account.add(i * 100);
    account.print();
}
