#include <iostream>
#include <memory>
#include <vector>

class Account {
public:
    double rate() const { return 3.5; } // non-virtual: static binding
    virtual double vrate() const { return 3.5; }
    virtual ~Account() = default;
};

class SeniorAccount : public Account {
private:
    double* history;

public:
    SeniorAccount() : history(new double[100]{}) {}
    double rate() const { return 4.0; } // hides Account::rate()
    double vrate() const override { return 4.0; }
    ~SeniorAccount() override { delete[] history; }
};

int main() {
    SeniorAccount senior;
    Account* ptr = &senior;
    Account slicedCopy = senior;

    std::cout << ptr->rate() << ' '
              << ptr->vrate() << ' '
              << slicedCopy.vrate() << '\n';

    std::vector<std::unique_ptr<Account>> accounts;
    accounts.push_back(std::make_unique<Account>());
    accounts.push_back(std::make_unique<SeniorAccount>());

    std::cout << "Stored account rates:\n";
    for (const auto& account : accounts) {
        std::cout << account->vrate() << '\n';
    }
    return 0;
}
