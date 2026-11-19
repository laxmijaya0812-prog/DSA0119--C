#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

int main() {
    int hour;

    while (true) {
        std::cout << "Enter hour (0-23): ";

        if (std::cin >> hour && hour >= 0 && hour <= 23) {
            break;
        }

        std::cout << "Invalid hour. Please enter a number from 0 to 23.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    long long farePaise = 3600;

    std::cout << "Formatted fare: Rs "
              << farePaise / 100 << '.'
              << std::setw(2) << std::setfill('0')
              << farePaise % 100
              << std::setfill(' ') << '\n';

    std::cout << "Peak: "
              << std::boolalpha
              << ((hour >= 8 && hour < 10) ||
                  (hour >= 17 && hour < 20))
              << '\n';
}
