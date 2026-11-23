// b2_alt_optional.cpp -- B2 alternative concept: std::optional<double> (no value = rejected)
// versus exceptions, 1,000,000 claims with 0 %, 1 % and 50 % invalid.
//   g++ -std=c++17 -O2 -Wall -Wextra b2_alt_optional.cpp
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <optional>
#include <stdexcept>
#include <utility>
#include <vector>

class ClaimError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

__attribute__((noinline)) std::optional<double> approveOpt(double billed, double cover) noexcept
{
    if (billed < 0 || cover < 0 || billed > cover) return std::nullopt;   // the reason is lost
    return billed;
}
__attribute__((noinline)) double approveEx(double billed, double cover)
{
    if (billed < 0 || cover < 0) throw std::invalid_argument("negative amount");
    if (billed > cover) throw ClaimError("exceeds cover");
    return billed;
}

template <class F>
double medianMs(F&& f, int trials = 5)
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
    std::cout << "sizeof(double) = " << sizeof(double) << ", sizeof(std::optional<double>) = " << sizeof(std::optional<double>) << '\n';
    std::cout << "invalid%   optional(ms)   exceptions(ms)   exceptions/optional   rejected(opt)  rejected(ex)\n";
    for (int pct : {0, 1, 50}) {
        std::vector<std::pair<double, double>> claims(n);
        std::mt19937 rng(99);
        for (auto& c : claims) {
            const bool invalid = int(rng() % 100) < pct;
            c = invalid ? std::pair<double, double>{60000, 50000} : std::pair<double, double>{40000, 50000};
        }
        std::size_t rejOpt = 0, rejEx = 0;
        double sumOpt = 0, sumEx = 0;
        volatile double sink = 0;
        const double tOpt = medianMs([&] {
            rejOpt = 0; sumOpt = 0;
            for (const auto& c : claims) {
                if (auto r = approveOpt(c.first, c.second)) sumOpt += *r; else ++rejOpt;
            }
            sink = sumOpt;
        });
        const double tEx = medianMs([&] {
            rejEx = 0; sumEx = 0;
            for (const auto& c : claims) {
                try { sumEx += approveEx(c.first, c.second); }
                catch (const ClaimError&) { ++rejEx; }
            }
            sink = sumEx;
        });
        std::cout << std::setw(5) << pct << "%   " << std::fixed << std::setprecision(2) << std::setw(12) << tOpt
                  << std::setw(16) << tEx << std::setw(19) << tEx / tOpt << "x" << std::setw(15) << rejOpt << std::setw(14) << rejEx
                  << (sumOpt == sumEx && rejOpt == rejEx ? "" : "  MISMATCH") << '\n';
    }
}
