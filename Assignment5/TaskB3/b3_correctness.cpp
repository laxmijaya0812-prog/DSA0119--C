// b3_correctness.cpp -- B3.1: the same 10 bills totalled by virtual functions, a type field + switch,
// and (alternative concept) std::variant. All three must agree with the hand total.
//   g++ -std=c++17 -Wall -Wextra b3_correctness.cpp
#include "b3_bills.hpp"

#include <cmath>
#include <iomanip>
#include <iostream>

using namespace b3;

int main()
{
    const Spec specs[10] = {
        {Kind::OP, 500, 1200, 0},       {Kind::IP, 3, 2500, 40000}, {Kind::DC, 30000, 0, 0}, {Kind::EM, 3000, 4, 1250},
        {Kind::OP, 800, 0, 0},          {Kind::IP, 5, 3000, 0},     {Kind::DC, 18000, 0, 0}, {Kind::EM, 2000, 2, 1500},
        {Kind::OP, 300, 4500, 0},       {Kind::IP, 2, 4000, 120000}};
    const double hand[10] = {1700, 47500, 30000, 8000, 800, 15000, 18000, 5000, 4800, 128000};
    const char* names[] = {"OP", "IP", "DC", "EM"};

    std::vector<std::unique_ptr<VBill>> v;
    std::vector<TBill> t;
    std::vector<Var> w;
    for (const Spec& s : specs) {
        v.push_back(makeV(s));
        t.push_back({s.kind, s.a, s.b, s.c});
        w.push_back(makeVar(s));
    }

    std::cout << " #  kind   hand     virtual   switch    variant   agree\n";
    double sv = 0, st = 0, sw = 0, sh = 0;
    int bad = 0;
    for (int i = 0; i < 10; ++i) {
        const double a = v[i]->total(), b = totalOf(t[i]), c = totalOf(w[i]);
        const bool ok = a == hand[i] && b == hand[i] && c == hand[i];
        bad += ok ? 0 : 1;
        sv += a; st += b; sw += c; sh += hand[i];
        std::cout << std::setw(2) << i + 1 << "  " << std::left << std::setw(5) << names[int(specs[i].kind)] << std::right
                  << std::setw(8) << hand[i] << std::setw(10) << a << std::setw(10) << b << std::setw(10) << c << "   "
                  << (ok ? "yes" : "NO") << '\n';
    }
    std::cout << "    total" << std::setw(8) << sh << std::setw(10) << sv << std::setw(10) << st << std::setw(10) << sw << "   "
              << (sv == sh && st == sh && sw == sh && bad == 0 ? "ALL AGREE" : "MISMATCH") << '\n';
    return bad == 0 ? 0 : 1;
}
