// a1_space_demo.cpp -- A1 alternative concept: one Bill struct with a type field + switch,
// versus an abstract base class with virtual functions. Same data, same totals, different cost.
//   g++ -std=c++17 -Wall -Wextra a1_space_demo.cpp
#include <iostream>
#include <memory>
#include <vector>

// ---- (a) abstract base class ---------------------------------------------------------
struct Bill {
    virtual ~Bill() = default;
    virtual double total() const noexcept = 0;
};
struct OutPatient : Bill {
    double consultation, tests;
    OutPatient(double c, double t) : consultation(c), tests(t) {}
    double total() const noexcept override { return consultation + tests; }
};
struct InPatient : Bill {
    int days;
    double perDay, surgery;
    InPatient(int d, double r, double s) : days(d), perDay(r), surgery(s) {}
    double total() const noexcept override { return days * perDay + surgery; }
};
struct DayCare : Bill {
    double price;
    explicit DayCare(double p) : price(p) {}
    double total() const noexcept override { return price; }
};
struct Emergency : Bill {
    double base;
    int hours;
    double rate;
    Emergency(double b, int h, double r) : base(b), hours(h), rate(r) {}
    double total() const noexcept override { return base + hours * rate; }
};

// ---- (b) one struct with a type field and a switch -----------------------------------
enum class Kind { OP, IP, DC, EM };
struct TaggedBill {
    Kind kind;
    double a, b, c;   // OP: a=consultation b=tests | IP: a=days b=perDay c=surgery
                      // DC: a=price | EM: a=base b=hours c=rate  (fields reused by every kind)
};
double total(const TaggedBill& t)
{
    switch (t.kind) {
        case Kind::OP: return t.a + t.b;
        case Kind::IP: return t.a * t.b + t.c;
        case Kind::DC: return t.a;
        case Kind::EM: return t.a + t.b * t.c;
    }
    return 0;   // unreachable; a NEW Kind added to the enum is silently ignored here unless every switch is edited
}

int main()
{
    std::vector<std::unique_ptr<Bill>> poly;
    poly.push_back(std::make_unique<OutPatient>(500, 1200));
    poly.push_back(std::make_unique<InPatient>(3, 2500, 40000));
    poly.push_back(std::make_unique<DayCare>(30000));
    poly.push_back(std::make_unique<Emergency>(3000, 4, 1250));
    const TaggedBill tagged[] = {{Kind::OP, 500, 1200, 0}, {Kind::IP, 3, 2500, 40000}, {Kind::DC, 30000, 0, 0},
                                 {Kind::EM, 3000, 4, 1250}};

    double sp = 0, st = 0;
    for (const auto& b : poly) sp += b->total();
    for (const auto& t : tagged) st += total(t);
    std::cout << "virtual total = " << sp << ", switch total = " << st << (sp == st ? "  (agree)\n" : "  (DIFFER)\n");

    std::cout << "sizeof(OutPatient) = " << sizeof(OutPatient) << "  sizeof(InPatient) = " << sizeof(InPatient)
              << "  sizeof(DayCare) = " << sizeof(DayCare) << "  sizeof(Emergency) = " << sizeof(Emergency) << '\n';
    std::cout << "sizeof(TaggedBill) = " << sizeof(TaggedBill) << " (same for every kind)\n";
    std::cout << "sizeof(std::unique_ptr<Bill>) = " << sizeof(std::unique_ptr<Bill>) << " (one pointer per bill in the virtual design)\n";
}
