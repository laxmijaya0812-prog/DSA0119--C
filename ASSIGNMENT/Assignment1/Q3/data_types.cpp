#include <climits>
#include <iomanip>
#include <iostream>
#include <limits>

int main() {
    bool concession = true;
    char zoneCode = 'M';
    int hour = 18;
    long long balancePaise = 10000LL;
    unsigned int journeyCount = 3000000U;
    float sensorValue = 12.5f;
    double rupees = 100.25;
    auto fare = 2000LL;

    std::cout << std::boolalpha;
    std::cout << "sizeof(bool) = " << sizeof(bool) << '\n';
    std::cout << "sizeof(char) = " << sizeof(char) << '\n';
    std::cout << "sizeof(int) = " << sizeof(int) << '\n';
    std::cout << "sizeof(unsigned int) = " << sizeof(unsigned int) << '\n';
    std::cout << "sizeof(long long) = " << sizeof(long long) << '\n';
    std::cout << "sizeof(float) = " << sizeof(float) << '\n';
    std::cout << "sizeof(double) = " << sizeof(double) << '\n';

    std::cout << "balance paise = " << balancePaise << '\n';
    std::cout << "journeys = " << journeyCount << '\n';
    std::cout << "hour = " << hour << '\n';
    std::cout << "zone code = " << zoneCode << '\n';
    std::cout << "concession = " << concession << '\n';
    std::cout << "float value = " << sensorValue << '\n';
    std::cout << "double value = " << rupees << '\n';
    std::cout << "auto fare = " << fare << '\n';

    std::cout << "INT_MAX = " << INT_MAX << '\n';
    std::cout << "LLONG_MAX = " << LLONG_MAX << '\n';

    double sum = 0.0;
    for (int i = 0; i < 1000000; ++i) {
        sum += 0.10;
    }
    std::cout << std::setprecision(17)
              << "double 1,000,000 x 0.10 = " << sum << '\n';
}
