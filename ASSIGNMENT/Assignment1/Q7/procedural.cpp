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

bool canTravel(long long balance, long long f) {
    return balance - f >= 5000;
}

void deduct(long long& balance, long long f) {
    if (!canTravel(balance, f)) {
        std::cout << "Denied\n";
        return;
    }
    balance -= f;
    std::cout << "Deducted " << f << " paise; balance = "
              << balance << " paise\n";
}

int main() {
    long long balance = 10000;
    deduct(balance, fare(Zone::Outer, true));
    deduct(balance, fare(Zone::Inner, false));
}
