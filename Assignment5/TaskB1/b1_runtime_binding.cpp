// b1_runtime_binding.cpp -- B1.3: prove runtime binding.
//   g++ -std=c++17 -Wall -Wextra -DVIRT=virtual b1_runtime_binding.cpp   (late binding)
//   g++ -std=c++17 -Wall -Wextra -DVIRT=         b1_runtime_binding.cpp   (early binding)
#include <iostream>
#include <string>

#ifndef VIRT
#define VIRT virtual
#endif

class Bill {
public:
    virtual ~Bill() = default;
    VIRT double total() const { return 0; }          // placeholder body so it compiles both ways
    VIRT std::string type() const { return "Bill"; }
};
class OutPatient : public Bill {
public:
    OutPatient(double c, double t) : c_(c), t_(t) {}
    double total() const { return c_ + t_; }          // same signature: overrides when virtual, hides otherwise
    std::string type() const { return "OP"; }
private:
    double c_, t_;
};
class InPatient : public Bill {
public:
    InPatient(int d, double r, double s) : d_(d), r_(r), s_(s) {}
    double total() const { return d_ * r_ + s_; }
    std::string type() const { return "IP"; }
private:
    int d_;
    double r_, s_;
};

int main()
{
    OutPatient op(500, 1200);
    InPatient ip(3, 2500, 40000);
    Bill* bills[] = {&op, &ip};

    std::cout << "Through derived objects (static type known):\n";
    std::cout << "  " << op.type() << " " << op.total() << "   " << ip.type() << " " << ip.total() << '\n';

    std::cout << "Through Bill* pointers:\n";
    double sum = 0;
    for (const Bill* b : bills) {
        std::cout << "  " << b->type() << " " << b->total() << '\n';
        sum += b->total();
    }
    std::cout << "Day total through Bill*: " << sum << '\n';
}
