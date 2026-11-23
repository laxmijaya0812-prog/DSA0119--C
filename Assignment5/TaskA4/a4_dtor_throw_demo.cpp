// a4_dtor_throw_demo.cpp -- A4.2: why destructors must not throw.
// A destructor throws while the stack is already being unwound for another exception:
// two active exceptions cannot be handled, so the runtime calls std::terminate().
//   g++ -std=c++17 -Wall -Wextra a4_dtor_throw_demo.cpp ; ./a.out   (expected: abort, exit code 134)
#include <iostream>
#include <stdexcept>

struct BadResource {
    ~BadResource() noexcept(false)                       // explicitly allowed to throw
    {
        std::cout << "~BadResource: throwing a second exception" << std::endl;
        throw std::runtime_error("thrown from a destructor");
    }
};

int main()
{
    try {
        BadResource r;
        std::cout << "throwing the first exception" << std::endl;
        throw std::logic_error("first error");
    } catch (const std::exception& e) {
        std::cout << "caught: " << e.what() << '\n';   // never reached
    }
    std::cout << "program continues\n";                 // never reached
}
