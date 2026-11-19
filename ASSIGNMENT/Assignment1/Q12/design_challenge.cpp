#include <iomanip>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

enum class Zone { Inner, Middle, Outer, Airport };

long long baseFare(Zone z) {
    switch (z) {
        case Zone::Inner: return 1000;
        case Zone::Middle: return 2000;
        case Zone::Outer: return 3000;
        case Zone::Airport: return 6000;
    }
    return -1;
}

bool isPeak(int hour) {
    return (hour >= 8 && hour < 10) ||
           (hour >= 17 && hour < 20);
}

long long fare(Zone zone, int hour, bool weekend) {
    long long result = baseFare(zone);

    // Weekend discount does not combine with peak surcharge.
    if (weekend) {
        result = result * 90 / 100;
    } else if (isPeak(hour)) {
        result += result * 20 / 100;
    }

    return result;
}

class MonthlyPass {
    std::set<Zone> zones;

public:
    MonthlyPass(Zone a, Zone b) {
        zones.insert(a);
        zones.insert(b);
    }

    bool covers(Zone z) const {
        return zones.count(z) != 0;
    }
};

class SmartCard {
    long long balancePaise;

public:
    explicit SmartCard(long long balance) : balancePaise(balance) {}

    bool canTravel(long long f) const {
        return balancePaise - f >= 5000;
    }

    bool deduct(long long f) {
        if (!canTravel(f)) return false;
        balancePaise -= f;
        return true;
    }

    long long balance() const { return balancePaise; }
};

std::string money(long long p) {
    std::ostringstream oss;
    oss << "Rs " << p / 100 << '.'
        << std::setw(2) << std::setfill('0')
        << p % 100;
    return oss.str();
}

int main() {
    std::cout << "Airport off-peak: "
              << fare(Zone::Airport, 12, false) / 100.0
              << " rupees\n";

    std::cout << "Airport peak: "
              << fare(Zone::Airport, 18, false) / 100.0
              << " rupees\n";

    std::cout << "Airport weekend: "
              << fare(Zone::Airport, 18, true) / 100.0
              << " rupees\n";

    MonthlyPass pass(Zone::Inner, Zone::Airport);

    std::cout << std::boolalpha
              << "Pass covers Inner: " << pass.covers(Zone::Inner) << '\n'
              << "Pass covers Outer: " << pass.covers(Zone::Outer) << '\n';

    SmartCard card(10000);
    std::cout << "Pass holder can be checked before fare deduction.\n";
    std::cout << "Current card balance: Rs "
              << card.balance() / 100 << '\n';
}
