#include <iostream>
#include <iomanip>
#include <string>

class SavingsAccount {
private:
    std::string accountNumber;
    long long balance;
    double transactions[10];
    int count;

    void addTransaction(double amount);

public:
    SavingsAccount(const std::string& number, long long openingBalance)
        : accountNumber(number), balance(openingBalance), count(0) {}

    void deposit(long long amount);
    bool withdraw(long long amount);
    void miniStatement() const;
};

void SavingsAccount::addTransaction(double amount) {
    if (count < 10) {
        transactions[count++] = amount;
    } else {
        for (int i = 1; i < 10; ++i)
            transactions[i - 1] = transactions[i];
        transactions[9] = amount;
    }
}

void SavingsAccount::deposit(long long amount) {
    if (amount > 0) {
        balance += amount;
        addTransaction(static_cast<double>(amount));
    }
}

bool SavingsAccount::withdraw(long long amount) {
    if (amount <= 0 || amount > balance) return false;
    balance -= amount;
    addTransaction(-static_cast<double>(amount));
    return true;
}

void SavingsAccount::miniStatement() const {
    std::cout << "Account: " << accountNumber << '\n';
    std::cout << "Balance: " << balance << '\n';
    std::cout << "Last transactions:\n";
    for (int i = 0; i < count; ++i)
        std::cout << std::fixed << std::setprecision(2) << transactions[i] << '\n';
}

int main() {
    SavingsAccount account("SB1001", 10000);
    account.deposit(500);
    account.withdraw(200);
    account.deposit(1000);
    account.miniStatement();
}
