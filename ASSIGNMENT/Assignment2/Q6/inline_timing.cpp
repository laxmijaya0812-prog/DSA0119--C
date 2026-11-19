#include <chrono>
#include <iostream>

class Time {
    int h, m;
public:
    Time(int hh, int mm) : h(hh), m(mm) {}
    friend inline int minutesBetween(const Time& a, const Time& b);
};

inline int minutesBetween(const Time& a, const Time& b) {
    int x = a.h * 60 + a.m;
    int y = b.h * 60 + b.m;
    int d = x - y;
    return d < 0 ? -d : d;
}

int main() {
    Time a(9, 45), b(13, 10);
    volatile long long total = 0;
    const int N = 10000000;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; ++i) total += minutesBetween(a, b);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "Total: " << total << '\n';
    std::cout << "Time(ms): " << elapsed.count() << '\n';
}
