#include <cstring>
#include <iostream>

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
        if (this == &other) return *this;

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
        std::size_t n1 = std::strlen(data);
        std::size_t n2 = std::strlen(other.data);

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

    friend std::ostream& operator<<(std::ostream& os, const Text& t) {
        return os << t.data;
    }
};

int main() {
    Text a("Hello");
    Text b(" World");
    Text c = a + b;
    std::cout << c << '\n';
    std::cout << std::boolalpha << (c == Text("Hello World")) << '\n';
}
