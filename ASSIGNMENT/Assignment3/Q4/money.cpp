#include <iomanip>
#include <iostream>
#include <stdexcept>

class Money {
    long long paise;

public:
    Money(long long rupees = 0, int p = 0)
        : paise(rupees * 100 + p) {}

    Money operator+(const Money& other) const {
        Money result;
        result.paise = paise + other.paise;
        return result;
    }

    Money operator-(const Money& other) const {
        Money result;
        result.paise = paise - other.paise;
        return result;
    }

    Money& operator+=(const Money& other) {
        paise += other.paise;
        return *this;
    }

    bool operator==(const Money& other) const {
        return paise == other.paise;
    }

    bool operator<(const Money& other) const {
        return paise < other.paise;
    }

    friend std::ostream& operator<<(std::ostream& os, const Money& m) {
        const long long rupees = m.paise / 100;
        const long long p = m.paise % 100;

        os << "Rs " << rupees << '.'
           << std::setw(2) << std::setfill('0') << p
           << std::setfill(' ');
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Money& m) {
        long long rupees;
        int p;
        if (is >> rupees >> p) {
            m.paise = rupees * 100 + p;
        }
        return is;
    }
};

int main() {
    Money a(120, 75), b(79, 25);
    Money c = a + b;

    std::cout << c << '\n'
              << std::boolalpha << (c == Money(200)) << '\n';

    Money carry(0, 75);
    carry += Money(0, 50);
    std::cout << "Carry test: " << carry << '\n';

    Money negative = Money(5) - Money(8);
    std::cout << "Negative test: " << negative << '\n';

    std::cout << "Comparison 120.75 < 200.00: "
              << (a < Money(200)) << '\n';
}
