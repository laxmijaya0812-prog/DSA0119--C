// predict_a2_nonvirtual_dtor.cpp -- A2 variant: virtual removed from ~Bill (undefined behaviour)
#include <iostream>

struct Bill {
    ~Bill() { std::cout << "~Bill "; }
    virtual double total() const = 0;
    void show() const { std::cout << total() << ' '; }
};
struct OPBill : Bill {
    ~OPBill() { std::cout << "~OPBill "; }
    double total() const override { return 500; }
};

int main()
{
    Bill* b = new OPBill;
    b->show();
    delete b;
    std::cout << '\n';
}
