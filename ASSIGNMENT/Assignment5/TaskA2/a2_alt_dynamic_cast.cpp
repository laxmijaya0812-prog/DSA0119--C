// a2_alt_dynamic_cast.cpp -- A2 alternative concept: total() is NOT virtual; the caller finds the
// derived type with a chain of dynamic_cast tests, versus one virtual call.
//   g++ -std=c++17 -O2 -Wall -Wextra a2_alt_dynamic_cast.cpp
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

struct BillV {                       // design 1: virtual
    virtual ~BillV() = default;
    virtual double total() const noexcept = 0;
};
struct BillD {                       // design 2: polymorphic base only so that dynamic_cast works
    virtual ~BillD() = default;
};
#define DEFINE_KINDS(BASE, OVERRIDE_V)                                                                   \
    struct OP_##BASE : BASE { double c, t; OP_##BASE(double c_, double t_) : c(c_), t(t_) {}              \
        double total() const noexcept OVERRIDE_V { return c + t; } };                                    \
    struct IP_##BASE : BASE { int d; double r, s; IP_##BASE(int d_, double r_, double s_) : d(d_), r(r_), s(s_) {} \
        double total() const noexcept OVERRIDE_V { return d * r + s; } };                                \
    struct DC_##BASE : BASE { double p; explicit DC_##BASE(double p_) : p(p_) {}                          \
        double total() const noexcept OVERRIDE_V { return p; } };                                        \
    struct EM_##BASE : BASE { double b; int h; double r; EM_##BASE(double b_, int h_, double r_) : b(b_), h(h_), r(r_) {} \
        double total() const noexcept OVERRIDE_V { return b + h * r; } };
DEFINE_KINDS(BillV, override)
DEFINE_KINDS(BillD, )

double totalByCast(const BillD* b) noexcept        // up to k = 4 casts per bill
{
    if (auto* p = dynamic_cast<const OP_BillD*>(b)) return p->total();
    if (auto* p = dynamic_cast<const IP_BillD*>(b)) return p->total();
    if (auto* p = dynamic_cast<const DC_BillD*>(b)) return p->total();
    if (auto* p = dynamic_cast<const EM_BillD*>(b)) return p->total();
    return 0;
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
    const std::size_t n = 1'000'000;
    std::vector<std::unique_ptr<BillV>> v;
    std::vector<std::unique_ptr<BillD>> d;
    std::mt19937 rng(2024);
    auto next = [&] { return rng() % 1000; };
    for (std::size_t i = 0; i < n; ++i) {
        const double a = next(), b = next(), c = next();
        switch (rng() % 4) {                       // random mix: type is unpredictable
            case 0: v.push_back(std::make_unique<OP_BillV>(a, b)); d.push_back(std::make_unique<OP_BillD>(a, b)); break;
            case 1: v.push_back(std::make_unique<IP_BillV>(int(a) % 10, b, c)); d.push_back(std::make_unique<IP_BillD>(int(a) % 10, b, c)); break;
            case 2: v.push_back(std::make_unique<DC_BillV>(a)); d.push_back(std::make_unique<DC_BillD>(a)); break;
            default: v.push_back(std::make_unique<EM_BillV>(a, int(b) % 10, c)); d.push_back(std::make_unique<EM_BillD>(a, int(b) % 10, c)); break;
        }
    }
    double sv = 0, sd = 0;
    volatile double sink = 0;
    const double tv = medianMs([&] { sv = 0; for (const auto& b : v) sv += b->total(); sink = sv; });
    const double td = medianMs([&] { sd = 0; for (const auto& b : d) sd += totalByCast(b.get()); sink = sd; });
    std::cout << "bills: " << n << " (random mix of k = 4 kinds)\n";
    std::cout << "virtual call            : " << tv << " ms  (" << tv * 1e6 / n << " ns/bill)\n";
    std::cout << "dynamic_cast chain      : " << td << " ms  (" << td * 1e6 / n << " ns/bill)\n";
    std::cout << "slowdown                : " << td / tv << " x\n";
    std::cout << "totals equal            : " << (sv == sd ? "yes" : "no") << "\n";
    std::cout << "sizeof(polymorphic OP) = " << sizeof(OP_BillV) << " (includes the 8-byte vtable pointer; payload = "
              << sizeof(double) * 2 << ")\n";
}
