// a3_alt_stdfunction.cpp -- A3 alternative concept: payers as std::function / function pointer
// (policy chosen at run time) instead of an abstract Payer class.
//   g++ -std=c++17 -O2 -Wall -Wextra a3_alt_stdfunction.cpp
#include "billing.hpp"

#include <chrono>
#include <functional>

using namespace hosp;

using PayerFn = std::function<double(const Bill&)>;

PayerFn makeInsuranceFn(double coverLimit, double copay)       // data lives in the lambda capture
{
    return [coverLimit, copay](const Bill& b) {
        const double payable = b.total() * (100.0 - copay) / 100.0;
        if (payable > coverLimit) throw ClaimExceedsCoverError("claim exceeds policy cover");
        return payable;
    };
}

struct Policy { double coverLimit, copay; };                   // plain function pointer: data must travel separately
double insuranceFp(const Policy& p, const Bill& b)
{
    const double payable = b.total() * (100.0 - p.copay) / 100.0;
    if (payable > p.coverLimit) throw ClaimExceedsCoverError("claim exceeds policy cover");
    return payable;
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
    std::vector<std::unique_ptr<Bill>> bills;
    for (int i = 0; i < 1000; ++i) bills.push_back(std::make_unique<OutPatientBill>("P" + std::to_string(i), 500 + i, 1200));

    const Insurance abstractPayer(1e9, 20);
    const Payer* viaVirtual = &abstractPayer;
    const PayerFn viaFunction = makeInsuranceFn(1e9, 20);
    double (*viaPointer)(const Policy&, const Bill&) = &insuranceFp;
    const Policy policy{1e9, 20};

    // same answers?
    double a = 0, b = 0, c = 0;
    for (const auto& x : bills) { a += viaVirtual->amountCovered(*x); b += viaFunction(*x); c += viaPointer(policy, *x); }
    std::cout << "sums: virtual=" << money(a) << " std::function=" << money(b) << " fn-pointer=" << money(c)
              << ((a == b && b == c) ? "  (all agree)\n" : "  (DIFFER)\n");

    const int passes = 10000;   // 1000 bills x 10000 passes = 10,000,000 calls
    volatile double sink = 0;
    const double tv = medianMs([&] { double s = 0; for (int p = 0; p < passes; ++p) for (const auto& x : bills) s += viaVirtual->amountCovered(*x); sink = s; });
    const double tf = medianMs([&] { double s = 0; for (int p = 0; p < passes; ++p) for (const auto& x : bills) s += viaFunction(*x); sink = s; });
    const double tp = medianMs([&] { double s = 0; for (int p = 0; p < passes; ++p) for (const auto& x : bills) s += viaPointer(policy, *x); sink = s; });
    const double calls = 1000.0 * passes;
    std::cout << "10,000,000 calls   virtual: " << tv << " ms (" << tv * 1e6 / calls << " ns/call)\n";
    std::cout << "                   std::function: " << tf << " ms (" << tf * 1e6 / calls << " ns/call)\n";
    std::cout << "                   function pointer: " << tp << " ms (" << tp * 1e6 / calls << " ns/call)\n";
    std::cout << "sizeof(Insurance object) = " << sizeof(Insurance) << " (vptr + 2 doubles)\n";
    std::cout << "sizeof(std::function<double(const Bill&)>) = " << sizeof(PayerFn) << " (+ heap block only if the capture is large)\n";
    std::cout << "sizeof(function pointer) = " << sizeof(viaPointer) << " (+ sizeof(Policy) = " << sizeof(Policy) << " kept elsewhere)\n";
}
