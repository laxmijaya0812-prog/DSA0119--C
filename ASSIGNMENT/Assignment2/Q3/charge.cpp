#include <iostream>
#include <string>

struct Parcel {
    double weightKg;
    bool express;
    bool insured;
};

double charge(double weightKg, bool express = false, bool insured = false) {
    double amount = weightKg * 50.0;
    if (express) amount += 40.0;
    if (insured) amount += 25.0;
    return amount;
}

double charge(int weightGrams, bool express = false) {
    return charge(weightGrams / 1000.0, express, false);
}

double charge(const Parcel& p) {
    return charge(p.weightKg, p.express, p.insured);
}

int main() {
    std::cout << "charge(2)    = " << charge(2) << '\n';
    std::cout << "charge(2.0f) = " << charge(static_cast<double>(2.0f)) << '\n';
    std::cout << "charge(2.0)  = " << charge(2.0) << '\n';

    std::cout << "grams        = " << charge(2000, true) << '\n';

    Parcel p{2.0, true, true};
    std::cout << "parcel       = " << charge(p) << '\n';

    std::cout << "full         = " << charge(2.0, true, true) << '\n';
}
