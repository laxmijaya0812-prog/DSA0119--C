#include <iostream>
#include <string>

enum class Zone { Inner, Middle, Outer };

constexpr long long INNER_PAISE = 1000;
constexpr long long MIDDLE_PAISE = 2000;
constexpr long long OUTER_PAISE = 3000;
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;

int main() {
    Zone zone = Zone::Middle;
    std::string name = "MetroCard";
    int hour = 9;
    bool peak = (hour >= 8 && hour < 10) || (hour >= 17 && hour < 20);

    std::cout << "Name: " << name << '\n';
    std::cout << "Peak: " << std::boolalpha << peak << '\n';
    std::cout << "Middle fare: Rs " << MIDDLE_PAISE / 100 << '\n';
    std::cout << "Minimum balance: Rs " << MIN_BALANCE_PAISE / 100 << '\n';

    if (zone == Zone::Middle) {
        std::cout << "Zone enum class selected: Middle\n";
    }
}
