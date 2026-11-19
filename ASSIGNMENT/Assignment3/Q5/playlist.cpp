#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class Time {
    int totalSeconds;

public:
    explicit Time(int seconds = 0) : totalSeconds(seconds) {}

    Time& operator++() {
        ++totalSeconds;
        return *this;
    }

    Time operator++(int) {
        Time old = *this;
        ++totalSeconds;
        return old;
    }

    bool operator==(const Time& other) const {
        return totalSeconds == other.totalSeconds;
    }

    bool operator<(const Time& other) const {
        return totalSeconds < other.totalSeconds;
    }

    void print() const {
        int h = totalSeconds / 3600;
        int m = (totalSeconds % 3600) / 60;
        int s = totalSeconds % 60;
        std::cout << h << ':' << (m < 10 ? "0" : "") << m
                  << ':' << (s < 10 ? "0" : "") << s;
    }
};

class Playlist {
    std::vector<std::string> tracks;

public:
    Playlist(std::initializer_list<std::string> values) : tracks(values) {}

    std::string& operator[](std::size_t i) {
        if (i >= tracks.size()) {
            throw std::out_of_range("playlist index out of range");
        }
        return tracks[i];
    }

    const std::string& operator[](std::size_t i) const {
        if (i >= tracks.size()) {
            throw std::out_of_range("playlist index out of range");
        }
        return tracks[i];
    }

    std::size_t size() const { return tracks.size(); }
};

int main() {
    Time t(3598);
    Time a = t++;
    Time b = ++t;

    std::cout << "a = ";
    a.print();
    std::cout << "\nb = ";
    b.print();
    std::cout << "\nt = ";
    t.print();
    std::cout << '\n';

    Playlist p{"Track A", "Track B", "Track C"};
    std::cout << "Playlist[1] = " << p[1] << '\n';

    try {
        std::cout << p[static_cast<std::size_t>(-1)] << '\n';
    } catch (const std::out_of_range& e) {
        std::cout << "Negative-index test: " << e.what() << '\n';
    }

    try {
        std::cout << p[p.size()] << '\n';
    } catch (const std::out_of_range& e) {
        std::cout << "size-index test: " << e.what() << '\n';
    }

    Time rollover(3599);
    ++rollover;
    std::cout << "Roll-over: ";
    rollover.print();
    std::cout << '\n';
}
