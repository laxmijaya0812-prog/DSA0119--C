// b3_benchmark.cpp -- B3.2 + alternative concept: time to total n bills with
//   virtual functions, a type field + switch, and std::variant + std::visit.
//   g++ -std=c++17 -O2 -Wall -Wextra b3_benchmark.cpp
// Each cell is the median of 5 trials; ms is per full pass over all n bills.
#include "b3_bills.hpp"

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>

using namespace b3;

template <class F>
double perPassMs(std::size_t n, F&& pass)
{
    const std::size_t passes = std::max<std::size_t>(1, 20'000'000 / n);   // at least ~20M bill visits per trial
    std::vector<double> trials;
    for (int t = 0; t < 5; ++t) {
        const auto a = std::chrono::steady_clock::now();
        for (std::size_t p = 0; p < passes; ++p) pass();
        const auto b = std::chrono::steady_clock::now();
        trials.push_back(std::chrono::duration<double, std::milli>(b - a).count() / double(passes));
    }
    std::sort(trials.begin(), trials.end());
    return trials[2];
}

struct Row { double ms[3]; double sum[3]; };

Row measure(std::size_t n, bool grouped)
{
    const std::vector<Spec> specs = generate(n, grouped);
    Row r{};
    volatile double sink = 0;
    {   // (1) virtual
        std::vector<std::unique_ptr<VBill>> v;
        v.reserve(n);
        for (const Spec& s : specs) v.push_back(makeV(s));
        double s = 0;
        r.ms[0] = perPassMs(n, [&] { s = 0; for (const auto& b : v) s += b->total(); sink = s; });
        r.sum[0] = s;
    }
    {   // (2) switch
        std::vector<TBill> t;
        t.reserve(n);
        for (const Spec& s : specs) t.push_back({s.kind, s.a, s.b, s.c});
        double s = 0;
        r.ms[1] = perPassMs(n, [&] { s = 0; for (const TBill& b : t) s += totalOf(b); sink = s; });
        r.sum[1] = s;
    }
    {   // (3) variant
        std::vector<Var> w;
        w.reserve(n);
        for (const Spec& s : specs) w.push_back(makeVar(s));
        double s = 0;
        r.ms[2] = perPassMs(n, [&] { s = 0; for (const Var& b : w) s += totalOf(b); sink = s; });
        r.sum[2] = s;
    }
    return r;
}

int main()
{
    std::cout << "sizeof: virtual objects = " << sizeof(VOP) << "/" << sizeof(VIP) << "/" << sizeof(VDC) << "/" << sizeof(VEM)
              << " (+8-byte pointer in the vector), TBill = " << sizeof(TBill) << ", std::variant = " << sizeof(Var)
              << " (largest alternative " << std::max({sizeof(POP), sizeof(PIP), sizeof(PDC), sizeof(PEM)}) << " + tag)\n";
    for (bool grouped : {false, true}) {
        std::cout << "\n== " << (grouped ? "bills grouped by type (predictable branches)" : "bills in random order (mixed day)") << " ==\n";
        std::cout << "        n |  virtual ms  ns/bill | switch ms   ns/bill | variant ms  ns/bill | sums equal\n";
        for (std::size_t n : {std::size_t(1000), std::size_t(1000000), std::size_t(10000000)}) {
            const Row r = measure(n, grouped);
            std::cout << std::setw(9) << n << " |" << std::fixed << std::setprecision(4);
            for (int k = 0; k < 3; ++k)
                std::cout << std::setw(12) << r.ms[k] << std::setprecision(2) << std::setw(8) << r.ms[k] * 1e6 / double(n)
                          << std::setprecision(4) << (k < 2 ? " |" : " | ");
            std::cout << (r.sum[0] == r.sum[1] && r.sum[1] == r.sum[2] ? "yes" : "NO") << '\n';
        }
    }
}
