#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Cart {
    std::string items[10];
    double prices[10]{};
    int n = 0;
    double discount = 0.0;

public:
    Cart& add(const std::string& item, double price) {
        if (n < 10) {
            items[n] = item;
            prices[n] = price;
            ++n;
        }
        return *this;
    }

    Cart& setDiscount(double discount) {
        this->discount = discount;
        return *this;
    }

    double total() const {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            sum += prices[i];
        }
        return sum * (100.0 - discount) / 100.0;
    }

    void display() const {
        for (int i = 0; i < n; ++i) {
            std::cout << items[i] << ": " << prices[i] << '\n';
        }
        std::cout << "Total: " << total() << '\n';
    }
};

int main() {
    // (a) Direct object access
    Cart direct;
    direct.add("Rice", 60).add("Dal", 120);

    // (b) Pointer access using ->
    Cart* pointer = &direct;
    pointer->add("Oil", 180);

    // (c) Reference access
    Cart& reference = direct;
    reference.setDiscount(10);

    // (d) Dynamically allocated object
    Cart* dynamicCart = new Cart;
    dynamicCart->add("Milk", 50).setDiscount(5);
    std::cout << "Dynamic cart total: " << dynamicCart->total() << '\n';
    delete dynamicCart;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Direct/pointer/reference cart total: "
              << pointer->total() << '\n';

    // Array of pointers and highest-total search
    Cart customerA;
    customerA.add("Book", 100);
    Cart customerB;
    customerB.add("Bag", 500).setDiscount(10);
    Cart customerC;
    customerC.add("Pen", 50).add("Notebook", 70);

    Cart* activeCarts[] = {&customerA, &customerB, &customerC};
    const int count = 3;
    int highestIndex = 0;
    for (int i = 1; i < count; ++i) {
        if (activeCarts[i]->total() > activeCarts[highestIndex]->total()) {
            highestIndex = i;
        }
    }
    std::cout << "Highest cart total: "
              << activeCarts[highestIndex]->total() << '\n';
    return 0;
}
