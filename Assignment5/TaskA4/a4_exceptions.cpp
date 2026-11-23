// a4_exceptions.cpp -- A4: error list, exception hierarchy and exception specifications.
//   g++ -std=c++17 -Wall -Wextra a4_exceptions.cpp
#include "billing.hpp"

#include <cstdlib>
#include <cxxabi.h>
#include <limits>
#include <new>
#include <type_traits>
#include <typeinfo>

using namespace hosp;

std::string demangle(const char* mangled)
{
    int status = 0;
    char* p = abi::__cxa_demangle(mangled, nullptr, nullptr, &status);
    std::string s = (status == 0 && p) ? p : mangled;
    std::free(p);
    return s;
}

template <class F>
void scenario(const char* label, F&& f)
{
    try {
        f();
        std::cout << std::left << std::setw(26) << label << "no exception\n";
    } catch (const std::exception& e) {
        std::cout << std::left << std::setw(26) << label << std::setw(28) << demangle(typeid(e).name())
                  << " ClaimError:" << (dynamic_cast<const ClaimError*>(&e) ? "yes" : "no ")
                  << " BillingError:" << (dynamic_cast<const BillingError*>(&e) ? "yes" : "no ")
                  << " what(): " << e.what() << '\n';
    }
}

int main()
{
    PatientRegistry registry;
    registry.add("Ravi");
    const BillingService service(registry);

    std::cout << "--- A4.1 every error in the case study, thrown and caught as std::exception ---\n";
    scenario("negative charge", [] { OutPatientBill b("Ravi", -500, 100); });
    scenario("claim above cover", [&] {
        const InPatientBill b("Ravi", 3, 2500, 40000, 2500);
        service.settle(b, Insurance(1000, 0));
    });
    scenario("unknown package", [&] {
        const DayCareBill b("Ravi", 30000, "DC-UNKNOWN");
        service.settle(b, GovernmentScheme({{"DC-CATARACT", 25000}}));
    });
    scenario("missing patient record", [&] {
        const OutPatientBill b("Nobody", 500, 100);
        service.settle(b, SelfPay());
    });
    scenario("text instead of number", [] { (void)std::stod("abc"); });
    scenario("allocation failure", [] {
        volatile std::size_t n = std::numeric_limits<std::size_t>::max() / 4;   // n * sizeof(double) overflows
        double* p = new double[n];   // throws std::bad_array_new_length (a std::bad_alloc)
        delete[] p;
    });

    std::cout << "\n--- A4.2 exception specifications (compile-time checks) ---\n";
    std::cout << std::boolalpha;
    std::cout << "Bill::total() is noexcept                      : " << noexcept(std::declval<const Bill&>().total()) << '\n';
    std::cout << "Bill::type() is noexcept                       : " << noexcept(std::declval<const Bill&>().type()) << '\n';
    std::cout << "Bill destructor is noexcept                    : " << std::is_nothrow_destructible_v<Bill> << '\n';
    std::cout << "Payer destructor is noexcept                   : " << std::is_nothrow_destructible_v<Payer> << '\n';
    std::cout << "BillingError copy-constructor is noexcept      : " << std::is_nothrow_copy_constructible_v<BillingError> << '\n';
    std::cout << "OutPatientBill constructor is noexcept         : "
              << std::is_nothrow_constructible_v<OutPatientBill, std::string, double, double> << "  (validates, so it may throw)\n";
    std::cout << "Payer::amountCovered() is noexcept             : " << noexcept(std::declval<const Payer&>().amountCovered(std::declval<const Bill&>()))
              << "  (may throw ClaimError)\n";
}
