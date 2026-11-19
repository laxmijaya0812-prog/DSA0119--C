#include <iostream>

class Time {
    int h, m;

public:
    Time(int hh, int mm) : h(hh), m(mm) {}

    friend int minutesBetween(const Time& a, const Time& b);
    friend bool earlier(const Time& a, const Time& b);
};

inline int minutesBetween(const Time& a, const Time& b) {
    int first = a.h * 60 + a.m;
    int second = b.h * 60 + b.m;
    int difference = first - second;
    return difference < 0 ? -difference : difference;
}

inline bool earlier(const Time& a, const Time& b) {
    return (a.h * 60 + a.m) < (b.h * 60 + b.m);
}

int main() {
    Time departure(9, 45), arrival(13, 10);
    std::cout << minutesBetween(departure, arrival) << ' '
              << std::boolalpha << earlier(arrival, departure) << '\n';

    Time noon1(11, 59), noon2(12, 1);
    std::cout << minutesBetween(noon1, noon2) << ' '
              << earlier(noon1, noon2) << '\n';

    Time same(8, 30), sameAgain(8, 30);
    std::cout << minutesBetween(same, sameAgain) << ' '
              << earlier(same, sameAgain) << '\n';
}
