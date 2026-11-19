#include <iostream>
#include <stdexcept>
#include <string>

class Book {
    std::string title;
    std::string author;
    int copies;
    double price;

    static int validCopies(int value) {
        if (value < 0) {
            throw std::invalid_argument("copies cannot be negative");
        }
        return value;
    }

public:
    Book() : Book("", "", 0, 0.0) {}

    explicit Book(const std::string& t) : Book(t, "Unknown", 1, 0.0) {}

    Book(const std::string& t, int c) : Book(t, "Unknown", validCopies(c), 0.0) {}

    Book(const std::string& t, const std::string& a, int c, double p)
        : title(t), author(a), copies(validCopies(c)), price(p) {}

    void print() const {
        std::cout << "Title: " << title
                  << ", Author: " << author
                  << ", Copies: " << copies
                  << ", Price: Rs " << price << '\n';
    }
};

int main() {
    try {
        Book empty;
        Book donated("C++ Basics");
        Book limited("Algorithms", 5);
        Book full("Modern C++", "Bjarne", 3, 599.0);

        std::cout << "Constructed books:\n";
        empty.print();
        donated.print();
        limited.print();
        full.print();

        try {
            Book invalid("Invalid Book", -2);
            invalid.print();
        } catch (const std::exception& e) {
            std::cout << "Negative-copy test: " << e.what() << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}
