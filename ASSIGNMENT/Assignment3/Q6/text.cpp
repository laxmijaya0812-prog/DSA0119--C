#include <cstring>
#include <iostream>
#include <stdexcept>

class Text {
    char* data;

public:
    Text(const char* s = "")
        : data(new char[std::strlen(s) + 1]) {
        std::strcpy(data, s);
    }

    Text(const Text& other)
        : data(new char[std::strlen(other.data) + 1]) {
        std::strcpy(data, other.data);
    }

    Text& operator=(const Text& other) {
        if (this == &other) {
            return *this;
        }

        char* newData = new char[std::strlen(other.data) + 1];
        std::strcpy(newData, other.data);

        delete[] data;
        data = newData;
        return *this;
    }

    ~Text() {
        delete[] data;
    }

    Text operator+(const Text& other) const {
        const std::size_t n1 = std::strlen(data);
        const std::size_t n2 = std::strlen(other.data);

        Text result;
        delete[] result.data;
        result.data = new char[n1 + n2 + 1];

        std::strcpy(result.data, data);
        std::strcat(result.data, other.data);
        return result;
    }

    bool operator==(const Text& other) const {
        return std::strcmp(data, other.data) == 0;
    }

    bool operator!=(const Text& other) const {
        return !(*this == other);
    }

    friend std::ostream& operator<<(std::ostream& os, const Text& t) {
        return os << t.data;
    }
};

int main() {
    Text a("Hello");
    Text b(" World");
    Text c = a + b;

    std::cout << "Concatenation: " << c << '\n';
    std::cout << std::boolalpha
              << "Equality: " << (c == Text("Hello World")) << '\n';

    Text empty("");
    std::cout << "Empty text length test: " << (empty == Text("")) << '\n';

    c = c; // self-assignment
    std::cout << "After self-assignment: " << c << '\n';

    Text longText("A message for the SMS gateway");
    Text copy = longText;
    std::cout << "Independent copy: " << copy << '\n';
}
