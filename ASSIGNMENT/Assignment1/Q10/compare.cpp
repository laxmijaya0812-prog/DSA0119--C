#include <iostream>
#include <iomanip>
#include <limits>

int main() {
    const long long journeys[] = {20000LL, 500000LL, 3000000LL};
    const double fare = 10.10;

    std::cout << std::fixed << std::setprecision(10);

    for (long long n : journeys) {
        long long integerPaise = 1010LL * n;
        double doubleRupees = fare * static_cast<double>(n);

        std::cout << "Journeys: " << n << '\n';
        std::cout << "Integer paise total: " << integerPaise << '\n';
        std::cout << "Double rupee total: " << doubleRupees << '\n';
        std::cout << "Difference in rupees: "
                  << doubleRupees - integerPaise / 100.0 << '\n';
    }

    int intTotal = 0;
    int sampleFare = 3600;

    for (int i = 0; i < 3000000; ++i) {
        intTotal += sampleFare;
    }

    std::cout << "int total after 3,000,000 x Rs 36: "
              << intTotal << '\n';
    std::cout << "INT_MAX: " << std::numeric_limits<int>::max() << '\n';
}
