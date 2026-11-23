// b1_alt_templates.cpp -- B1 alternative concept: compile-time polymorphism (templates),
// one container per bill type, versus virtual calls through Bill*.
//   g++ -std=c++17 -O2 -Wall -Wextra b1_alt_templates.cpp
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

// ---- (a) virtual: one list of base-class pointers ---------------------------------
struct Bill {
    virtual ~Bill() = default;
    virtual double total() const noexcept = 0;
};
struct OutPatient : Bill {
    double c, t;
    OutPatient(double c_, double t_) : c(c_), t(t_) {}
    double total() const noexcept override { return c + t; }
};
struct InPatient : Bill {
    int days;
    double perDay, surgery;
    InPatient(int d, double r, double s) : days(d), perDay(r), surgery(s) {}
    double total() const noexcept override { return days * perDay + surgery; }
};

// ---- (b) templates: plain types, one container per type ---------------------------
struct OP { double c, t;                      double total() const noexcept { return c + t; } };
struct IP { int days; double perDay, surgery; double total() const noexcept { return days * perDay + surgery; } };

template <class B>
double sumAll(const std::vector<B>& v) noexcept
{
    double s = 0;
    for (const B& b : v) s += b.total();      // resolved at compile time, can be inlined
    return s;
}

template <class F>
double medianMs(F&& f, int trials = 7)
{
    std::vector<double> t;
    for (int i = 0; i < trials; ++i) {
        const auto a = std::chrono::steady_clock::now();
        f();
        const auto b = std::chrono::steady_clock::now();
        t.push_back(std::chrono::duration<double, std::milli>(b - a).count());
    }
    std::sort(t.begin(), t.end());
    return t[t.size() / 2];
}

int main()
{
    const std::size_t n = 1'000'000;            // random mix of the two kinds
    std::vector<std::unique_ptr<Bill>> poly;
    std::vector<OP> ops;
    std::vector<IP> ips;
    std::mt19937 rng(12345);
    auto next = [&] { return double(rng() % 1000); };
    for (std::size_t i = 0; i < n; ++i) {
        if (rng() % 2 == 0) {
            const double a = next(), b = next();
            poly.push_back(std::make_unique<OutPatient>(a, b));
            ops.push_back({a, b});
        } else {
            const int d = int(next()) % 10;
            const double r = next(), s = next();
            poly.push_back(std::make_unique<InPatient>(d, r, s));
            ips.push_back({d, r, s});
        }
    }

    volatile double sink = 0;
    double polySum = 0, tmplSum = 0;
    const double tPoly = medianMs([&] { polySum = 0; for (const auto& b : poly) polySum += b->total(); sink = polySum; });
    const double tTmpl = medianMs([&] { tmplSum = sumAll(ops) + sumAll(ips); sink = tmplSum; });

    std::cout << "bills: " << n << " (" << ops.size() << " OP + " << ips.size() << " IP, random order)\n";
    std::cout << "sizeof(OutPatient virtual) = " << sizeof(OutPatient) << ", sizeof(OP template type) = " << sizeof(OP) << '\n';
    std::cout << "sizeof(InPatient virtual)  = " << sizeof(InPatient) << ", sizeof(IP template type) = " << sizeof(IP) << '\n';
    std::cout << "virtual  (one list, Bill*)        : " << tPoly << " ms  (" << tPoly * 1e6 / n << " ns/bill)  sum=" << polySum << '\n';
    std::cout << "template (one vector per type)    : " << tTmpl << " ms  (" << tTmpl * 1e6 / n << " ns/bill)  sum=" << tmplSum << '\n';
    std::cout << "sums equal: " << (polySum == tmplSum ? "yes" : "no") << '\n';
    // What is lost: the two vectors cannot be merged into one std::vector<?>; the arrival order of bills
    // is gone and every new bill type needs a new container and a new call in the code that totals them.
}
