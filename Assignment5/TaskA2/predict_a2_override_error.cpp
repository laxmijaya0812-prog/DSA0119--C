// predict_a2_override_error.cpp -- INTENTIONALLY FAILS TO COMPILE: virtual removed from ~Bill but override kept
#include <iostream>

struct Bill {
    ~Bill() { std::cout << "~Bill "; }
    virtual double total() const = 0;
    void show() const { std::cout << total() << ' '; }
};
struct OPBill : Bill {
    ~OPBill() override { std::cout << "~OPBill "; }
    double total() const override { return 500; }
};

int main()
{
    Bill* b = new OPBill;
    b->show();
    delete b;
    std::cout << '\n';
}
