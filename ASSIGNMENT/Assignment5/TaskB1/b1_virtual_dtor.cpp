// b1_virtual_dtor.cpp -- B1.4: prove the virtual destructor.
//   g++ -std=c++17 -Wall -Wextra -g -fsanitize=address -DVDTOR=virtual b1_virtual_dtor.cpp   (correct)
//   g++ -std=c++17 -Wall -Wextra -g -fsanitize=address -DVDTOR=         b1_virtual_dtor.cpp   (undefined behaviour)
#include <iostream>
#include <string>
#include <vector>

#ifndef VDTOR
#define VDTOR virtual
#endif

class Bill {
public:
    explicit Bill(std::string p) : patient_(std::move(p)) {}
    VDTOR ~Bill() { std::cout << "  ~Bill\n"; }
    virtual double total() const = 0;
private:
    std::string patient_;
};

class InPatient : public Bill {
public:
    InPatient(std::string p, int days, double rate)
        : Bill(std::move(p)), days_(days), rate_(rate), itemised_(100, rate) {}   // 100 doubles on the heap
    ~InPatient() { std::cout << "  ~InPatient (frees " << itemised_.size() * sizeof(double) << " heap bytes)\n"; }
    double total() const override { return days_ * rate_; }
private:
    int days_;
    double rate_;
    std::vector<double> itemised_;
};

int main()
{
    std::cout << "sizeof(Bill) = " << sizeof(Bill) << ", sizeof(InPatient) = " << sizeof(InPatient) << '\n';
    Bill* b = new InPatient("Meena", 3, 2500);
    std::cout << "total = " << b->total() << '\n';
    std::cout << "deleting through Bill*:\n";
    delete b;
    std::cout << "done\n";
}
