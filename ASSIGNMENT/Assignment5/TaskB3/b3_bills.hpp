// b3_bills.hpp -- the three representations compared in Task B3 (same data, same formulas):
//   (1) abstract base class + virtual functions
//   (2) one struct with a type field + switch
//   (3) std::variant of plain structs + std::visit
#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <random>
#include <variant>
#include <vector>

namespace b3 {

enum class Kind : std::uint8_t { OP, IP, DC, EM };

struct Spec {          // OP: a=consultation b=tests | IP: a=days b=perDay c=surgery | DC: a=price | EM: a=base b=hours c=rate
    Kind kind;
    double a, b, c;
};

// ---------------------------------------------------------------- (1) virtual
struct VBill {
    virtual ~VBill() = default;
    virtual double total() const noexcept = 0;
};
struct VOP final : VBill {
    double consultation, tests;
    VOP(double a, double b) : consultation(a), tests(b) {}
    double total() const noexcept override { return consultation + tests; }
};
struct VIP final : VBill {
    int days;
    double perDay, surgery;
    VIP(int d, double r, double s) : days(d), perDay(r), surgery(s) {}
    double total() const noexcept override { return days * perDay + surgery; }
};
struct VDC final : VBill {
    double price;
    explicit VDC(double p) : price(p) {}
    double total() const noexcept override { return price; }
};
struct VEM final : VBill {
    double base, hours, rate;
    VEM(double b, double h, double r) : base(b), hours(h), rate(r) {}
    double total() const noexcept override { return base + hours * rate; }
};
inline std::unique_ptr<VBill> makeV(const Spec& s)
{
    switch (s.kind) {
        case Kind::OP: return std::make_unique<VOP>(s.a, s.b);
        case Kind::IP: return std::make_unique<VIP>(int(s.a), s.b, s.c);
        case Kind::DC: return std::make_unique<VDC>(s.a);
        case Kind::EM: return std::make_unique<VEM>(s.a, s.b, s.c);
    }
    return nullptr;
}

// ---------------------------------------------------------------- (2) type field + switch
struct TBill {
    Kind kind;
    double a, b, c;
};
inline double totalOf(const TBill& t) noexcept
{
    switch (t.kind) {
        case Kind::OP: return t.a + t.b;
        case Kind::IP: return int(t.a) * t.b + t.c;
        case Kind::DC: return t.a;
        case Kind::EM: return t.a + t.b * t.c;
    }
    return 0;
}

// ---------------------------------------------------------------- (3) variant + visit
struct POP { double consultation, tests;       double total() const noexcept { return consultation + tests; } };
struct PIP { int days; double perDay, surgery; double total() const noexcept { return days * perDay + surgery; } };
struct PDC { double price;                     double total() const noexcept { return price; } };
struct PEM { double base, hours, rate;         double total() const noexcept { return base + hours * rate; } };
using Var = std::variant<POP, PIP, PDC, PEM>;
inline Var makeVar(const Spec& s)
{
    switch (s.kind) {
        case Kind::OP: return POP{s.a, s.b};
        case Kind::IP: return PIP{int(s.a), s.b, s.c};
        case Kind::DC: return PDC{s.a};
        case Kind::EM: return PEM{s.a, s.b, s.c};
    }
    return PDC{0};
}
inline double totalOf(const Var& v) noexcept
{
    return std::visit([](const auto& bill) noexcept { return bill.total(); }, v);
}

// ---------------------------------------------------------------- data generator
// grouped = false: kinds in random order (branches unpredictable, like a real mixed day's bills)
// grouped = true : kinds in four contiguous blocks (branches perfectly predictable)
inline std::vector<Spec> generate(std::size_t n, bool grouped)
{
    std::vector<Spec> specs(n);
    std::mt19937 rng(20240601u);                       // Mersenne Twister: no short-period patterns
    auto value = [&] { return double(1 + rng() % 1000); };
    for (std::size_t i = 0; i < n; ++i) {
        const Kind k = grouped ? Kind(i * 4 / n) : Kind(rng() % 4);
        const double a = value(), b = value(), c = value();
        specs[i] = (k == Kind::IP) ? Spec{k, double(1 + int(a) % 10), b, c} : Spec{k, a, b, c};
    }
    return specs;
}

}  // namespace b3
