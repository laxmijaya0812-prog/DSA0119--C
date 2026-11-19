#include <iostream>

enum class Zone { Inner, Middle, Outer };

long long fare(Zone zone, bool peak) {
    long long base = 0;

    switch (zone) {
        case Zone::Inner:  base = 1000; break;
        case Zone::Middle: base = 2000; break;
        case Zone::Outer:  base = 3000; break;
    }

    if (peak) base += base * 20 / 100;
    return base;
}

class SmartCard {
    long long balancePaise;

public:
    explicit SmartCard(long long initialBalance)
        : balancePaise(initialBalance) {}

    bool canTravel(long long f) const {
        return balancePaise - f >= 5000;
    }

    bool deduct(long long f) {
        if (!canTravel(f)) {
            return false;
        }
        balancePaise -= f;
        return true;
    }

    long long balance() const {
        return balancePaise;
    }
};

int main() {
    SmartCard card(10000);

    long long f1 = fare(Zone::Outer, true);
    std::cout << (card.deduct(f1) ? "Travel allowed" : "Denied")
              << ", balance = " << card.balance() << " paise\n";

    long long f2 = fare(Zone::Inner, false);
    std::cout << (card.deduct(f2) ? "Travel allowed" : "Denied")
              << ", balance = " << card.balance() << " paise\n";
}
