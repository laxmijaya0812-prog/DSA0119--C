#include <iomanip>
#include <iostream>

class Money {
    long long paise;

public:
    Money(long long rupees = 0, int p = 0) : paise(rupees * 100 + p) {}

    Money operator+(const Money& other) const {
        Money result;
        result.paise = paise + other.paise;
        return result;
    }

    bool operator==(const Money& other) const {
        return paise == other.paise;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        long long rupees = m.paise / 100;
        long long p = m.paise % 100;
        os << "Rs " << rupees << '.'
           << std::setw(2) << std::setfill('0') << p
           << std::setfill(' ');
        return os;
    }
};

int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;
    std::cout << c << '\n'
              << std::boolalpha << (c == Money(200)) << '\n';
}
