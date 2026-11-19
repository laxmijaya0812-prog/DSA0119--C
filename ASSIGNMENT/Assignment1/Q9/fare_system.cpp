#include <iomanip>
#include <iostream>
#include <string>

enum class Zone { Inner, Middle, Outer };

long long baseFare(Zone z) {
    switch (z) {
        case Zone::Inner: return 1000;
        case Zone::Middle: return 2000;
        case Zone::Outer: return 3000;
    }
    return -1;
}

bool isPeak(int hour) {
    return (hour >= 8 && hour < 10) ||
           (hour >= 17 && hour < 20);
}

long long calculateFare(Zone z, int hour, bool studentOrSenior) {
    long long fare = baseFare(z);

    if (fare < 0 || hour < 0 || hour > 23) {
        return -1;
    }

    if (isPeak(hour)) {
        fare += fare * 20 / 100;
    }

    if (studentOrSenior) {
        fare = fare * 50 / 100;
    }

    return fare;
}

std::string money(long long p) {
    return "Rs " + std::to_string(p / 100) + "." +
           (p % 100 < 10 ? "0" : "") + std::to_string(p % 100);
}

int main() {
    struct Test {
        std::string name;
        Zone zone;
        int hour;
        bool concession;
        long long expected;
    };

    Test tests[] = {
        {"Inner off-peak", Zone::Inner, 12, false, 1000},
        {"Middle off-peak", Zone::Middle, 12, false, 2000},
        {"Outer off-peak", Zone::Outer, 12, false, 3000},
        {"Outer peak", Zone::Outer, 18, false, 3600},
        {"Exactly 10:00", Zone::Outer, 10, false, 3000},
        {"Exactly 20:00", Zone::Outer, 20, false, 3000},
        {"Student peak", Zone::Middle, 9, true, 1200},
        {"Student off-peak", Zone::Middle, 12, true, 1000}
    };

    int passed = 0;
    for (const auto& t : tests) {
        long long actual =
            calculateFare(t.zone, t.hour, t.concession);

        bool ok = actual == t.expected;
        passed += ok;

        std::cout << std::left << std::setw(22)
                  << t.name
                  << " expected=" << money(t.expected)
                  << " actual=" << (actual >= 0 ? money(actual) : "INVALID")
                  << " -> " << (ok ? "PASS" : "FAIL")
                  << '\n';
    }

    std::cout << "Passed " << passed << "/" 
              << (sizeof(tests) / sizeof(tests[0])) << " tests\n";

    std::cout << "Boundary rule: 10:00 and 20:00 are off-peak.\n";
}
