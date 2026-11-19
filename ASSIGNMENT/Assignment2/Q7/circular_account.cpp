#include <iostream>
#include <iomanip>
#include <string>

class CircularAccount {
    static constexpr int CAPACITY = 10;
    std::string accountNumber;
    long long balance;
    double transactions[CAPACITY]{};
    int start = 0;
    int count = 0;

    void addTransaction(double amount) {
        int index;
        if (count < CAPACITY) {
            index = (start + count) % CAPACITY;
            ++count;
        } else {
            start = (start + 1) % CAPACITY;
            index = (start + count - 1) % CAPACITY;
        }
        transactions[index] = amount;
    }

public:
    CircularAccount(const std::string& number, long long openingBalance)
        : accountNumber(number), balance(openingBalance) {}

    void deposit(long long amount) {
        if (amount > 0) {
            balance += amount;
            addTransaction(static_cast<double>(amount));
        }
    }

    bool withdraw(long long amount) {
        if (amount <= 0 || amount > balance) return false;
        balance -= amount;
        addTransaction(-static_cast<double>(amount));
        return true;
    }

    void miniStatement() const {
        std::cout << "Account: " << accountNumber << '\n';
        std::cout << "Balance: " << balance << '\n';
        std::cout << "Last transactions:\n";

        for (int i = 0; i < count; ++i) {
            int index = (start + i) % CAPACITY;
            std::cout << std::fixed << std::setprecision(2)
                      << transactions[index] << '\n';
        }
    }
};

int main() {
    CircularAccount account("SB1001", 10000);

    for (int i = 1; i <= 12; ++i)
        account.deposit(i * 100);

    account.miniStatement();
}
