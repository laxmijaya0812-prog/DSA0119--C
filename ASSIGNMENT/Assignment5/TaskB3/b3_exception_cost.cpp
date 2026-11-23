// b3_exception_cost.cpp -- B3.3: cost of reporting errors by return codes versus exceptions,
// 1,000,000 bills with 0 %, 1 % and 50 % invalid (negative charge).
//   g++ -std=c++17 -O2 -Wall -Wextra b3_exception_cost.cpp
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

struct BillingError : std::runtime_error { using std::runtime_error::runtime_error; };
struct NegativeChargeError : BillingError { using BillingError::BillingError; };

enum class Status { Ok, NegativeCharge };

// ---- return-code design ----------------------------------------------------------------
__attribute__((noinline)) Status validateRC(double charge) noexcept
{
    return charge < 0 ? Status::NegativeCharge : Status::Ok;
}
__attribute__((noinline)) Status checkBillRC(double charge, double& out) noexcept   // one extra frame, forwards the code
{
    const Status s = validateRC(charge);
    if (s != Status::Ok) return s;
    out = charge;
    return Status::Ok;
}

// ---- exception design (same two frames) ---------------------------------------------------
__attribute__((noinline)) void validateEx(double charge)
{
    if (charge < 0) throw NegativeChargeError("negative charge");
}
__attribute__((noinline)) double checkBillEx(double charge)
{
    validateEx(charge);
    return charge;
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
    std::cout << "invalid%  invalid bills | return codes ms | exceptions ms | exceptions/codes | extra us per thrown exception\n";
    for (int pct : {0, 1, 50}) {
        std::vector<double> charges(n);
        std::mt19937 rng(7);
        std::size_t invalid = 0;
        for (auto& c : charges) {
            const bool bad = int(rng() % 100) < pct;
            c = bad ? -250.0 : 1000.0;
            invalid += bad;
        }
        double sumRC = 0, sumEx = 0;
        std::size_t badRC = 0, badEx = 0;
        volatile double sink = 0;
        const double tRC = medianMs([&] {
            sumRC = 0; badRC = 0;
            for (double c : charges) { double out = 0; if (checkBillRC(c, out) == Status::Ok) sumRC += out; else ++badRC; }
            sink = sumRC;
        });
        const double tEx = medianMs([&] {
            sumEx = 0; badEx = 0;
            for (double c : charges) { try { sumEx += checkBillEx(c); } catch (const NegativeChargeError&) { ++badEx; } }
            sink = sumEx;
        });
        std::cout << std::setw(7) << pct << "%  " << std::setw(13) << invalid << " |" << std::fixed << std::setprecision(3)
                  << std::setw(15) << tRC << " |" << std::setw(14) << tEx << " |" << std::setprecision(1) << std::setw(15)
                  << tEx / tRC << "x |";
        if (invalid) std::cout << std::setprecision(2) << std::setw(18) << (tEx - tRC) * 1000.0 / double(invalid);
        else std::cout << std::setw(18) << "n/a";
        std::cout << ((sumRC == sumEx && badRC == badEx) ? "" : "  MISMATCH") << '\n';
    }
}
